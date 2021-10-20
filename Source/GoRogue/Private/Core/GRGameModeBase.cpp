// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GRGameModeBase.h"
#include "Characters/GRCharacterAI.h"
#include "Components/GRAttributeComponent.h"

#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EngineUtils.h"

AGRGameModeBase::AGRGameModeBase()
{

}

void AGRGameModeBase::SpawnBotTimerElapsed()
{
	int32 NrOfAliveBots = 0;

	for (TActorIterator<AGRCharacterAI> It(GetWorld()); It; ++It)
	{
		AGRCharacterAI* Bot = *It;

		//UGRAttributeComponent* HealthComp = Cast<UGRAttributeComponent>(Bot->GetComponentByClass(UGRAttributeComponent::StaticClass()));
		UGRAttributeComponent* HealthComp = UGRAttributeComponent::GetAttributes(Bot);

		if (ensure(HealthComp) && HealthComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Find %i alive bots."), NrOfAliveBots);

	float MaxBotCount = 0.f;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AGRGameModeBase::OnQueryCompleted);
	}
}

void AGRGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed!"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}


void AGRGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AGRGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void AGRGameModeBase::KillAll()
{
	for (TActorIterator<AGRCharacterAI> It(GetWorld()); It; ++It)
	{
		AGRCharacterAI* Bot = *It;

		//UGRAttributeComponent* HealthComp = Cast<UGRAttributeComponent>(Bot->GetComponentByClass(UGRAttributeComponent::StaticClass()));
		UGRAttributeComponent* HealthComp = UGRAttributeComponent::GetAttributes(Bot);

		if (ensure(HealthComp) && HealthComp->IsAlive())
		{
			HealthComp->Kill(this); // Pass in player for kill credit.
		}
	}
}
