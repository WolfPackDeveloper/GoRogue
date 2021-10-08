// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GRProjectileDash.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AGRProjectileDash::AGRProjectileDash()
{
	MoveComp->InitialSpeed = 6000.f;
}


void AGRProjectileDash::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &AGRProjectileDash::Explode, DetonateDelay);
}


void AGRProjectileDash::Explode_Implementation()
{
	// Clear timer if the Explode was already called through another source like OnActorHit
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	MoveComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &AGRProjectileDash::TeleportInstigator, TeleportDelay);

	// Skip base implementation as it will destroy actor
	//Super::Explode_Implementation();
}

void AGRProjectileDash::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();

	if (ensure(ActorToTeleport))
	{
		// Keep instigator rotation, or it may end up jarring
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
}
