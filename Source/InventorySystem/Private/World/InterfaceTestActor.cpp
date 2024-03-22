// Fill out your copyright notice in the Description page of Project Settings.


#include "World/InterfaceTestActor.h"

// Sets default values
AInterfaceTestActor::AInterfaceTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	SetRootComponent(Mesh);

}

// Called when the game starts or when spawned
void AInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInterfaceTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInterfaceTestActor::InitiateFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void AInterfaceTestActor::TerminateFocus()
{
	if(Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void AInterfaceTestActor::InitiateInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling InitiateInteract override on the Interface Test Actor"));
}

void AInterfaceTestActor::TerminateInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling TerminateInteract override on the Interface Test Actor"));
}

void AInterfaceTestActor::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling Interact override on the Interface Test Actor"));
}

