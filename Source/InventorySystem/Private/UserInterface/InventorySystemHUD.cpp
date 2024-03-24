// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/InventorySystemHUD.h"
#include "UserInterface/MainMenu.h"
#include "UserInterface/Interaction/InteractionWidget.h"

AInventorySystemHUD::AInventorySystemHUD()
{
}

void AInventorySystemHUD::BeginPlay()
{
	Super::BeginPlay();

	if(MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AInventorySystemHUD::DisplayMenu()
{
}

void AInventorySystemHUD::HideMenu()
{
}

void AInventorySystemHUD::ShowInteractionWidget()
{
}

void AInventorySystemHUD::HideInteractionWidget()
{
}

void AInventorySystemHUD::UpdateInteractionWidget(const FInteractableData* InteractableData)
{
}

