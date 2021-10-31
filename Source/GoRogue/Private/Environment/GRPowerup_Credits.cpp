// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/GRPowerup_Credits.h"
#include "Core/GRPlayerState.h"



AGRPowerup_Credits::AGRPowerup_Credits()
{

}

void AGRPowerup_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (AGRPlayerState* PS = InstigatorPawn->GetPlayerState<AGRPlayerState>())
	{
		PS->AddCredits(CreditsAmount);
		HideAndCooldownPowerup();
	}
}
