// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GRGameplayInterface.h"
#include "GRItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class GOROGUE_API AGRItemChest : public AActor, public IGRGameplayInterface
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AGRItemChest();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

public:

	UPROPERTY(EditAnywhere)
	float TargetPitch = 110.f;
	
	// Interface implementation
	void Interact_Implementation(APawn* InstigationPawn);
};
