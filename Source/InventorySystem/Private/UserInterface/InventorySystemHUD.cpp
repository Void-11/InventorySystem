// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/InventorySystemHUD.h"

#include "Chaos/Levelset.h"
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
	if(MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AInventorySystemHUD::HideMenu()
{
	if(MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AInventorySystemHUD::ShowInteractionWidget()
{
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AInventorySystemHUD::HideInteractionWidget()
{
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AInventorySystemHUD::UpdateInteractionWidget(const FInteractableData* InteractableData)
{
	if(InteractionWidget)
	{
		if(InteractionWidget->GetVisibility() == (ESlateVisibility::Collapsed))
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);	
		}
	}
}

