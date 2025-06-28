// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/IS_PlayerController.h"

#include "Inventory.h"

void AIS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogInventory, Log, TEXT("BeginPlay for PlayerController"))
}
