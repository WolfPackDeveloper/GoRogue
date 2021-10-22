// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GRBTService_CheckHealth.h"
#include "Components/GRAttributeComponent.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UGRBTService_CheckHealth::UGRBTService_CheckHealth()
{
	//LowHealthFraction = 0.3f
}

void UGRBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ensure(AIPawn))
	{
		UGRAttributeComponent* HealthComp = UGRAttributeComponent::GetAttributes(AIPawn);
		if (ensure(HealthComp))
		{
			bool bLowHealth = (HealthComp->GetHealth() / HealthComp->GetHealthMax()) < LowHealthFraction;

			UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
			BlackBoardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);
		}
	}
}
