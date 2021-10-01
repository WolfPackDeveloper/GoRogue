// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/GRItemChest.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AGRItemChest::AGRItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

}

// Called when the game starts or when spawned
void AGRItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGRItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGRItemChest::Interact_Implementation(APawn* InstigationPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0.f, 0.f));
}

