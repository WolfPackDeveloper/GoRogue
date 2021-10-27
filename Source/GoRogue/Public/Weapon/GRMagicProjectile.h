// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GRProjectileBase.h"
#include "GameplayTagContainer.h"
#include "GRMagicProjectile.generated.h"

UCLASS()
class GOROGUE_API AGRMagicProjectile : public AGRProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGRMagicProjectile();

private:

	float DefaultSphereRadius = 20.f;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	

};
