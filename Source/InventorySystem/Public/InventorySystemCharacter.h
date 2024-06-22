// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Interfaces/InteractionInterface.h"
#include "InventorySystemCharacter.generated.h"


class UTimelineComponent;
class UItemBase;
class UInventoryComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class AInventorySystemHUD;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

USTRUCT()

struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionTimeCheck(0.0f)
	{
		
	};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionTimeCheck;
};

UCLASS(config=Game)
class AInventorySystemCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ToggleMenuAction;

public:
	AInventorySystemCharacter();

	bool bAiming;
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; };

	void UpdateInteractionWidget() const;

	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);

protected:

	//PROPERTIES AND VARIABLES
	
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;
	
	float InteractionFrequencyCheck;

	float InteractionDistanceCheck;

	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;

	UPROPERTY()
	AInventorySystemHUD* HUD;

	// timeline properties used for camera aiming transition
	UPROPERTY(VisibleAnywhere, Category="Character | Camera")
	FVector DefaultCameraLocation;
	UPROPERTY(VisibleAnywhere, Category="Character | Camera")
	FVector AimCameraLocation;

	TObjectPtr<UTimelineComponent> AimCameraTimeline;

	UPROPERTY(EditDefaultsOnly, Category="Character | Aim Timeline")
	TObjectPtr<UCurveFloat> AimCameraCurve;

	
	//FUNCTIONS
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

	void ToggleMenu();

	void Aim();
	void StopAiming();

	UFUNCTION()
	void UpdateCameraTimeline(const float TimelineValue) const;

	UFUNCTION()
	void CameraTimelineEnd();
	
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void InitiateInteract();
	void TerminateInteract();
	void Interact();

	
};

