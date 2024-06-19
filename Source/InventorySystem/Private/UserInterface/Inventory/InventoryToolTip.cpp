// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/InventoryToolTip.h"
#include "Components/TextBlock.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Items/ItemBase.h"

void UInventoryToolTip::NativeConstruct()
{
	Super::NativeConstruct();

	const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

	switch(ItemBeingHovered->ItemType)
	{
	case EItemType::Armor:
		break;
	case EItemType::Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorStrength->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Mundane:
		ItemType->SetText(FText::FromString("Mundane Item"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorStrength->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Quest:
		break;
	case EItemType::Shield:
		break;
	case EItemType::Spell:
		break;
	case EItemType::Weapon:
		break;

	default: ;
	}

	ItemName->SetText(ItemBeingHovered->TextData.Name);
	DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.DamageValue));
	ArmorStrength->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.ArmorStrength));
	UsageText->SetText(ItemBeingHovered->TextData.UsageText);
	ItemDescription->SetText(ItemBeingHovered->TextData.Description);

    const FString WeightInfo = {"Weight:" + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight())};
        
    StackWeightValue->SetText(FText::FromString(WeightInfo));
    
	if(ItemBeingHovered->NumericData.bIsStackable)
	{
	    const FString StackInfo = {"Max Stack Size:" + FString::FromInt(ItemBeingHovered->NumericData.MaxStackSize)};
                
		MaxStackSize->SetText(FText::FromString(StackInfo));
	}
	else
	{
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}

}
