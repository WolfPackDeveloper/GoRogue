// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GRTargetDummy.generated.h"

class UGRAttributeComponent;

UCLASS()
class GOROGUE_API AGRTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGRTargetDummy();

protected:

	UPROPERTY(VisibleAnywhere)
	UGRAttributeComponent* HealthComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UFUNCTION()
		void OnHealthChanged(AActor* InstigatorActor, UGRAttributeComponent* OwningComp, float NewHealth, float Delta);

};
