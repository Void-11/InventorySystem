#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

UENUM()
enum class EItemQuality : uint8
{
	LowerGrade UMETA(DisplayName = "Lower-Grade"),
	Ordinary UMETA(DisplayName = "Ordinary"),
	SemiGrade UMETA(DisplayName = "Semi-Grade"),
	UpperGrade UMETA(DisplayName = "Upper-Grade"),
	SpecialGrade UMETA(DisplayName = "Special-Grade")
	
};

UENUM()
enum class EItemType : uint8
{
	Armor UMETA(DisplayName = "Armor"),
	Consumable UMETA(DisplayName = "Consumable"),
	Mundane UMETA(DisplayName = "Mundane"),
	Quest UMETA(DisplayName = "Quest"),
	Shield UMETA(DisplayName = "Shield"),
	Spell UMETA(DisplayName = "Spell"),
	Weapon UMETA(DisplayName = "Weapon"),
	
};

USTRUCT()
struct FItemStatistics 
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	float ArmorStrength;

	UPROPERTY(EditAnywhere)
	float DamageValue;

	UPROPERTY(EditAnywhere)
	float RestorationAmount;

	UPROPERTY(EditAnywhere)
	float SellValue;
};

USTRUCT()
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FText InteractText;

	UPROPERTY(EditAnywhere)
	FText UsageText;
};

USTRUCT()
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	float Weight;

	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;

	UPROPERTY()
	bool bIsStackable;
	
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;
	
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData AssetData;
};