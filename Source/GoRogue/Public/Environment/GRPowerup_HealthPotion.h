// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GRPowerupActor.h"
#include "GRPowerup_HealthPotion.generated.h"

class UStaticMeshComponent;

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

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	// float healt amount?

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

};
