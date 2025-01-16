// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventorySystemCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Interfaces/InteractionInterface.h"
#include  "DrawDebugHelpers.h"
#include "Components/InventoryComponent.h"
#include "Components/TimelineComponent.h"
#include "UserInterface/InventorySystemHUD.h"
#include "World/Pickup.h"

class APickup;
DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AInventorySystemCharacter

AInventorySystemCharacter::AInventorySystemCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	PlayerInventory->SetSlotsCapacity(20);
	PlayerInventory->SetWeightCapacity(50.0f);

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AimCameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AimCameraTimeline"));
	DefaultCameraLocation = FVector{0.0f, 0.0f, 65.0f};
	AimCameraLocation = FVector{175.0f, 50.0f, 55.0f};
	CameraBoom->SocketOffset = DefaultCameraLocation;
	
	InteractionFrequencyCheck = 0.1;
	InteractionDistanceCheck = 227.0f;

	// capsule default dimensions = 34.0f, 88.0f
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	BaseEyeHeight = 77.0f;
}

void AInventorySystemCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//MainPlayerController = Cast<AInventorySystemPlayerController>(GetController());
	HUD = Cast<AInventorySystemHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	
	//if (MainPlayerController)
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		//if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		//	MainPlayerController->GetLocalPlayer()))
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	FOnTimelineFloat AimLerpAlphaValue;
	FOnTimelineEvent TimelineFinishedEvent;
	AimLerpAlphaValue.BindUFunction(this, FName("UpdateCameraTimeline"));
	TimelineFinishedEvent.BindUFunction(this, FName("CameraTimelineEnd"));

	if (AimCameraTimeline && AimCameraCurve)
	{
		AimCameraTimeline->AddInterpFloat(AimCameraCurve, AimLerpAlphaValue);
		AimCameraTimeline->SetTimelineFinishedFunc(TimelineFinishedEvent);
	}
}

void AInventorySystemCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(GetWorld()->TimeSince(InteractionData.LastInteractionTimeCheck) > InteractionFrequencyCheck)
	{
		PerformInteractionCheck();
	}
}

void AInventorySystemCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionTimeCheck = GetWorld()->GetTimeSeconds();

	FVector TraceStart;

	if (!bAiming)
	{
		InteractionDistanceCheck = 227.0f;
		TraceStart = GetPawnViewLocation();
	}
	else
	{
		InteractionDistanceCheck = 277.0f;
		TraceStart = FollowCamera->GetComponentLocation();
	}

	FVector TraceEnd{TraceStart + (GetViewRotation().Vector() * InteractionDistanceCheck)};

	if(float LookDirection = FVector::DotProduct(GetActorForwardVector(),GetViewRotation().Vector()); LookDirection > 0)
	{	
		DrawDebugLine(GetWorld(),TraceStart,TraceEnd,FColor::Purple,false,1.0f,0,2.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		if(FHitResult TraceHit; GetWorld()->LineTraceSingleByChannel(TraceHit,TraceStart,TraceEnd,ECC_Visibility,QueryParams))
		{
			if(TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{

				if(TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				if(TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}
		}
	}

	NoInteractableFound();
}

void AInventorySystemCharacter::FoundInteractable(AActor* NewInteractable)
{
	if(IsInteracting())
	{
		TerminateInteract();
	}

	if(InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->TerminateFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	
	TargetInteractable->InitiateFocus();
}

void AInventorySystemCharacter::NoInteractableFound()
{
	if(IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if(InteractionData.CurrentInteractable)
	{
		if(IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->TerminateFocus();
			TerminateInteract();
		}

		HUD->HideInteractionWidget();

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void AInventorySystemCharacter::InitiateInteract()
{
	//VERIFY NOTHING HAS CHANGED WITH THE INTERACTABLE STATE SINCE INTERACTION START
	PerformInteractionCheck();

	if(InteractionData.CurrentInteractable)
	{
		if(IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->InitiateInteract();

			if(FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration,0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
												this,
												&AInventorySystemCharacter::Interact,
												TargetInteractable->InteractableData.InteractionDuration,
												false);
			}
		}
	}
}

void AInventorySystemCharacter::TerminateInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if(IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->TerminateInteract();
	}
}

void AInventorySystemCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if(IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
}

void AInventorySystemCharacter::UpdateInteractionWidget() const
{
	if(IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void AInventorySystemCharacter::ToggleMenu()
{
	HUD->ToggleMenu();

	if (HUD->bIsMenuVisible)
	{
		StopAiming();
	}
}

void AInventorySystemCharacter::Aim()
{
	if (!HUD->bIsMenuVisible)
	{
		bAiming = true;
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;

		if (AimCameraTimeline)
			AimCameraTimeline->PlayFromStart();
	}
}

void AInventorySystemCharacter::StopAiming()
{
	if (bAiming)
	{
		bAiming = false;
		bUseControllerRotationYaw = false;
		HUD->HideCrosshair();
		GetCharacterMovement()->MaxWalkSpeed = 500.f;

		if (AimCameraTimeline)
			AimCameraTimeline->Reverse();
	}
}

void AInventorySystemCharacter::UpdateCameraTimeline(const float TimelineValue) const
{
	const FVector CameraLocation = FMath::Lerp(DefaultCameraLocation, AimCameraLocation, TimelineValue);
	CameraBoom->SocketOffset = CameraLocation;
}

void AInventorySystemCharacter::CameraTimelineEnd()
{
	if (AimCameraTimeline)
	{
		if (AimCameraTimeline->GetPlaybackPosition() != 0.0f)
		{
			HUD->ShowCrosshair();
		}
	}
}

void AInventorySystemCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation(GetActorLocation() + (GetActorForwardVector() * 50.0f));
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

		Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was somehow null!"));
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AInventorySystemCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInventorySystemCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInventorySystemCharacter::Look);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AInventorySystemCharacter::InitiateInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AInventorySystemCharacter::TerminateInteract);

		//Aim
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AInventorySystemCharacter::Aim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AInventorySystemCharacter::StopAiming);

		//MenuToggle
		EnhancedInputComponent->BindAction(ToggleMenuAction, ETriggerEvent::Started, this, &AInventorySystemCharacter::ToggleMenu);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	// PlayerInputComponent->BindAction("Interact",IE_Pressed,this,&AInventorySystemCharacter::InitiateInteract);
	// PlayerInputComponent->BindAction("Interact",IE_Released,this,&AInventorySystemCharacter::TerminateInteract);
	//
	// PlayerInputComponent->BindAction("ToggleMenu", IE_Pressed, this, &AInventorySystemCharacter::ToggleMenu);
}

void AInventorySystemCharacter::Move(const FInputActionValue& Value)
{ 
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AInventorySystemCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}