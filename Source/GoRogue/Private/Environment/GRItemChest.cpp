// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/GRItemChest.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGRItemChest::AGRItemChest()
{
 	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

}

void AGRItemChest::Interact_Implementation(APawn* InstigationPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0.f, 0.f));
}

