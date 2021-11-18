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

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly, SaveGame)
	bool bLidOpened = false;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	UFUNCTION()
	void OnRep_LidOpened();

public:

	UPROPERTY(EditAnywhere)
	float TargetPitch = 110.f;
	
	// GameplayInterface implementation
	void Interact_Implementation(APawn* InstigationPawn);
	void OnActorLoaded_Implementation();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
