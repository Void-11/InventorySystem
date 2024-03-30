// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystemCharacter.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Items/ItemBase.h"
#include "Pickup.generated.h"

//class UDataTable;
//class UItemBase;

UCLASS()
class INVENTORYSYSTEM_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	


	//FUNCTIONS
	APickup();

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);

	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);

	FORCEINLINE UItemBase* GetItemData() const { return ItemReference; };

	virtual void InitiateFocus() override;
	virtual void TerminateFocus() override;
	


protected:
	//PROPERTIES AND VARIABLES
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	FName DesiredItemID;
	
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;
	
	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Reference")
	int32 ItemQuantity;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;

	//FUNCTIONS
	virtual void BeginPlay() override;
	
	virtual void Interact(AInventorySystemCharacter* PlayerCharacter) override;

	void UpdateInteractableData();
	void TakePickup(const AInventorySystemCharacter* Taker);
};
