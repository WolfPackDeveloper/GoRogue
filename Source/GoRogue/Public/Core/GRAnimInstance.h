// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GRAnimInstance.generated.h"

class UGRActionComponent;

/**
 * 
 */
UCLASS()
class GOROGUE_API UGRAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:

	/* Is Pawn Stunned based on GameplayTag data. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsStunned;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UGRActionComponent* ActionComp;


	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};
