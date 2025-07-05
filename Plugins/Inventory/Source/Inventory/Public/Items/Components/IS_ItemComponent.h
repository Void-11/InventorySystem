// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IS_ItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UIS_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UIS_ItemComponent();

	FString GetPickupMessage() const { return PickupMessage; }

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString PickupMessage;
};
