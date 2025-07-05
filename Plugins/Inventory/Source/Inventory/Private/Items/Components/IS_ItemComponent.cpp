// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/IS_ItemComponent.h"

UIS_ItemComponent::UIS_ItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PickupMessage = FString("E - Pick Up");
}


