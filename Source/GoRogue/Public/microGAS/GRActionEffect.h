// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "microGAS/GRAction.h"
#include "GRActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class GOROGUE_API UGRActionEffect : public UGRAction
{
	GENERATED_BODY()
	
public:

	UGRActionEffect();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	float Duration;

	/* Time between ticks to apply effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	float Period;

	FTimerHandle DurationHandle;
	FTimerHandle PeriodHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodEffect(AActor* Instigator);
	virtual void ExecutePeriodEffect_Implementation(AActor* Instigator);

public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;
};
