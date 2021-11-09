// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GRPowerupActor.h"
#include "GRPowerup_Action.generated.h"

class UGRAction;

/**
 * 
 */
UCLASS()
class GOROGUE_API AGRPowerup_Action : public AGRPowerupActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "Powerup")
	TSubclassOf<UGRAction> ActionToGrant;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;


};
