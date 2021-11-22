// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GRPowerupActor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AGRPowerupActor::AGRPowerupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	SetReplicates(true);
}

void AGRPowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	// Set visibility on root and all children
	RootComponent->SetVisibility(bIsActive, true);
}

void AGRPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

void AGRPowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);

	FTimerHandle TimerHandle_RespawnTimer;
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &AGRPowerupActor::ShowPowerup, RespawnTime);
}

void AGRPowerupActor::SetPowerupState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_IsActive();
}

void AGRPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{

}

void AGRPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGRPowerupActor, bIsActive);
}
