// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GRPowerupActor.h"
#include "GRPowerup_Credits.generated.h"

/**
 * 
 */
UCLASS()
class GOROGUE_API AGRPowerup_Credits : public AGRPowerupActor
{
	GENERATED_BODY()
	
public:

	AGRPowerup_Credits();

protected:

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 CreditsAmount = 80;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;
};
