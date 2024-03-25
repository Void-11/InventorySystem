// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/MainMenu.h"

#include "InventorySystemCharacter.h"

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AInventorySystemCharacter>(GetOwningPlayerPawn());
}

bool UMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	//CAST OPERATION TO ITEM DRAG DROP, ENSURE PLAYER IS VALID, CALL DROP ITEM ON PLAYER
}
