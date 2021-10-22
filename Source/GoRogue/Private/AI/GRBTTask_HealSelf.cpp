// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GRBTTask_HealSelf.h"
#include "Components/GRAttributeComponent.h"

#include "AIController.h"

EBTNodeResult::Type UGRBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UGRAttributeComponent* HealthComp = UGRAttributeComponent::GetAttributes(MyPawn);
	
	if (ensure(HealthComp))
	{
		HealthComp->ApplyHealthChange(MyPawn, HealthComp->GetHealthMax());
	}

	return EBTNodeResult::Succeeded;
}
