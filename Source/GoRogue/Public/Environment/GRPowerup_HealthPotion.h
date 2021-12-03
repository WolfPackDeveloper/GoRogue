// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GRPowerupActor.h"
#include "GRPowerup_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class GOROGUE_API AGRPowerup_HealthPotion : public AGRPowerupActor
{
	GENERATED_BODY()
	
public:

	AGRPowerup_HealthPotion();

protected:

	UPROPERTY(EditAnywhere, Category = "Components")
	int32 CreditCost = 50;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractText_Implementation(APawn* InstigationPawn) override;

};
