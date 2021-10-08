// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/GRProjectileBase.h"
#include "GRProjectileDash.generated.h"

/**
 * 
 */
UCLASS()
class GOROGUE_API AGRProjectileDash : public AGRProjectileBase
{
	GENERATED_BODY()
	
public:
	AGRProjectileDash();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay = 0.2f;

	// Handle to cancel timer if we already hit something
	FTimerHandle TimerHandle_DelayedDetonate;

	virtual void BeginPlay() override;

	virtual void Explode_Implementation() override;

	void TeleportInstigator();

};
