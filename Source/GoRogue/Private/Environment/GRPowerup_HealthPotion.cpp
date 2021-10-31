// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/GRPowerup_HealthPotion.h"
#include "Components/GRAttributeComponent.h"
#include "Core/GRPlayerState.h"

//#include "Components/StaticMeshComponent.h"



AGRPowerup_HealthPotion::AGRPowerup_HealthPotion()
{

}


void AGRPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	UGRAttributeComponent* AttributeComp = UGRAttributeComponent::GetAttributes(InstigatorPawn);
	
	// Check if not already at max health
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (AGRPlayerState* PS = InstigatorPawn->GetPlayerState<AGRPlayerState>())
		{
			
			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
			{
				// Only activate if healed successfully
				HideAndCooldownPowerup();
			}
		}
	}
}