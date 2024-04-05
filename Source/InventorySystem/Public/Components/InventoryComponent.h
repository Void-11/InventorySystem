// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/ItemBase.h"
#include "InventoryComponent.generated.h"

class UItemBase;

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	Iar_NoItemAdded UMETA(DisplayName = "No item added"),
	Iar_PartialAmountItemAdded UMETA(DisplayName = "Partial item added"),
	Iar_AllItemAdded UMETA(DisplayName = "All of item added"),
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() :
	ActualAmountAdded(0),
	OperationResult(EItemAddResult::Iar_NoItemAdded),
	ResultMessage(FText::GetEmpty())
	{};

	// Actual amount of item that was added to the inventory
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded;

	// Enum representing the end state of an add item operation
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;

	// Informational message that can be passed with the result
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::Iar_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	};
	
	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::Iar_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	};
	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::Iar_AllItemAdded;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
	};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	//FUNCTIONS
	UInventoryComponent();

	UItemBase* FindMatchingItem(UItemBase* ItemIn) const;
	UItemBase* FindNextItemByID(UItemBase* ItemIn) const;
	UItemBase* FindNextPartialStack(UItemBase* ItemIn) const;
	FItemAddResult HandleAddItem(UItemBase* InputItem);
	void RemoveSingleInstanceOfItem(UItemBase* ItemIn);
	int32 RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove);
	void SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit);

	FORCEINLINE float GetInventoryTotalWeight() const { return InventoryTotalWeight; };
	FORCEINLINE float GetWeightCapacity() const { return InventoryWeightCapacity; };
	FORCEINLINE int32 GetSlotsCapacity() const { return InventorySlotsCapacity; };
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const { return InventoryContents; };

	FORCEINLINE void SetSlotsCapacity(const int32 NewSLotsCapacity) {};
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) {};

	
protected:

	//PROPERTIES AND VARIABLES
	float InventoryTotalWeight;
	int32 InventorySlotsCapacity;
	float InventoryWeightCapacity;

	TArray<TObjectPtr<UItemBase>> InventoryContents;

	//FUNCTIONS
	virtual void BeginPlay() override;



		
};
