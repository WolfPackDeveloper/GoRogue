// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GRBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class GOROGUE_API UGRBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	UGRBTTask_RangedAttack();

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletSpread = 2.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;
};
