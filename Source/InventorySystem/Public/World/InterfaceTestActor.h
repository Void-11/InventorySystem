// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "InterfaceTestActor.generated.h"

UCLASS()
class INVENTORYSYSTEM_API AInterfaceTestActor : public AActor, public  IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInterfaceTestActor();

protected:

	UPROPERTY(EditAnywhere,Category = "Test Actor")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditInstanceOnly, Category = "Test Actor")
	FInteractableData InstanceInteractableData;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void InitiateFocus() override;
	virtual void TerminateFocus() override;
	virtual void InitiateInteract() override;
	virtual void TerminateInteract() override;
	virtual void Interact(AInventorySystemCharacter* PlayerCharacter) override;
};
