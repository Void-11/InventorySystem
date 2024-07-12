// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryToolTip.generated.h"

class UTextBlock;
class UInventoryItemSlot;

UCLASS()
class INVENTORYSYSTEM_API UInventoryToolTip : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInventoryItemSlot> InventorySlotBeingHovered;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemType;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DamageValue;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ArmorStrength;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> UsageText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemDescription;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MaxStackSize;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> StackWeightValue;

protected:

	virtual void NativeConstruct() override;
};
