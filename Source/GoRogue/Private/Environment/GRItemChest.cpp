// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/GRItemChest.h"

#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AGRItemChest::AGRItemChest()
{
 	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	SetReplicates(true);
}

void AGRItemChest::Interact_Implementation(APawn* InstigationPawn)
{
	bLidOpened = !bLidOpened;
	// Manually called for the server
	OnRep_LidOpened();
}

void AGRItemChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0.f, 0.f));
}

void AGRItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void AGRItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGRItemChest, bLidOpened);
}
