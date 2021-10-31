// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GRGameplayInterface.h"
#include "GRPowerupActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS(ABSTRACT)
class GOROGUE_API AGRPowerupActor : public AActor, public IGRGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGRPowerupActor();

protected:

	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime = 10.f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UFUNCTION()
	void ShowPowerup();

	void HideAndCooldownPowerup();

	void SetPowerupState(bool bNewIsActive);

public:	

	void Interact_Implementation(APawn* InstigatorPawn) override;

};
