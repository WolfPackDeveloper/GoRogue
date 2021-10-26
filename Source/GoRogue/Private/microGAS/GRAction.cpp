// Fill out your copyright notice in the Description page of Project Settings.


#include "microGAS/GRAction.h"

void UGRAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
}

void UGRAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
}

UWorld* UGRAction::GetWorld() const
{
	// Outer si set when creating action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());

	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}
