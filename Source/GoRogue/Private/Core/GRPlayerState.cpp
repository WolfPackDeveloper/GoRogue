// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GRPlayerState.h"
#include "Core/GRSaveGame.h"

#include "Net/UnrealNetwork.h"

void AGRPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

 //void AGRPlayerState::MulticastCredits_Implementation(float NewCredits, float Delta)
 //{
 //	OnCreditsChanged.Broadcast(this, NewCredits, Delta);
 //}

int32 AGRPlayerState::GetCredits(int32 Delta) const
{
	return Credits;
}

void AGRPlayerState::AddCredits(int32 Delta)
{
	// Avoid user-error of adding a negative amount or zero
	if (!ensure(Delta > 0.0f))
	{
		return;
	}

	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool AGRPlayerState::RemoveCredits(int32 Delta)
{
	// Avoid user-error of adding a subtracting negative amount or zero
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}

	if (Credits < Delta)
	{
		// Not enough credits available
		return false;
	}

	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, -Delta);

	return true;
}

void AGRPlayerState::SavePlayerState_Implementation(UGRSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}

void AGRPlayerState::LoadPlayerState_Implementation(UGRSaveGame* SaveObject)
{
	if (SaveObject)
	{
		//Credits = SaveObject->Credits;
		// Makes sure we trigger credits changed event
		AddCredits(SaveObject->Credits);
	}
}

void AGRPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGRPlayerState, Credits);
}
