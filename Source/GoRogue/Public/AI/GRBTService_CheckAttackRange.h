// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GRBTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class GOROGUE_API UGRBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

public:

	UGRBTService_CheckAttackRange();

protected:

	/* Max desired attack range of AI pawn */
	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxAttackRange = 2000.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttackRangeKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
