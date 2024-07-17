// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/InventoryToolTip.h"
#include "Components/TextBlock.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Items/ItemBase.h"

void UInventoryToolTip::NativeConstruct()
{
	Super::NativeConstruct();

	if (const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference())
	{
		switch (ItemBeingHovered->ItemQuality)
		{
		case EItemQuality::LowerGrade:
			ItemType->SetColorAndOpacity(FLinearColor::Gray);
			break;
			
		case EItemQuality::Ordinary:
			ItemType->SetColorAndOpacity(FLinearColor::White);
			break;
			
		case EItemQuality::SemiGrade:
			ItemType->SetColorAndOpacity(FLinearColor(0.0f, 0.51f, 0.169f));
			break;
			
		case EItemQuality::UpperGrade:
			ItemType->SetColorAndOpacity(FLinearColor(0.0f, 0.4f, 0.75f));
			break;
			
		case EItemQuality::SpecialGrade:
			ItemType->SetColorAndOpacity(FLinearColor(1.0f, 0.45f, 0.0f));
			break;
			
		default: ;
		}
		
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
			const FString StackInfo = {"Max Stack Size: " + FString::FromInt(ItemBeingHovered->NumericData.MaxStackSize)};
                
			MaxStackSize->SetText(FText::FromString(StackInfo));
		}
		else
		{
			MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
