// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/GRPowerup_HealthPotion.h"
#include "Components/GRAttributeComponent.h"
#include "Core/GRPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

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

FText AGRPowerup_HealthPotion::GetInteractText_Implementation(APawn* InstigationPawn)
{
	UGRAttributeComponent* AttributeComp = UGRAttributeComponent::GetAttributes(InstigationPawn);

	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}

	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."), CreditCost);
}

#undef LOCTEXT_NAMESPACE
