// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "microGAS/GRActionEffect.h"
#include "GRActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class GOROGUE_API UGRActionEffect_Thorns : public UGRActionEffect
{
	GENERATED_BODY()
	
public:

	UGRActionEffect_Thorns();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectFraction = 0.2f;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UGRAttributeComponent* OwningComp, float NewHealth, float Delta);

public:

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;
};
