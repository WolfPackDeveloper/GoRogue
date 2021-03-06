// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GRGameModeBase.h"
#include "Core/GRPlayerState.h"
#include "Characters/GRCharacterAI.h"
#include "Characters/GRCharacterBase.h"
#include "Components/GRAttributeComponent.h"
#include "Core/GRSaveGame.h"
#include "Data/GRMonsterData.h"
#include "Interfaces/GRGameplayInterface.h"
#include "microGAS/GRActionComponent.h"
#include "../GoRogue.h"

#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Engine/AssetManager.h"
#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h" //Save Game
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

AGRGameModeBase::AGRGameModeBase()
{
	PlayerStateClass = AGRPlayerState::StaticClass();
	SlotName = "SaveGame01";
}

void AGRGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");

	if (SelectedSaveSlot.Len() > 0)
	{
		SlotName = SelectedSaveSlot;
	}

	LoadSaveGame();
}

void AGRGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}
	
	int32 NrOfAliveBots = 0;

	for (TActorIterator<AGRCharacterAI> It(GetWorld()); It; ++It)
	{
		AGRCharacterAI* Bot = *It;

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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AGRGameModeBase::OnBotSpawnQueryCompleted);
	}
}

void AGRGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed!"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows;
			MonsterTable->GetAllRows("", Rows);

			// Get random enemy.
			int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			UAssetManager* Manager = UAssetManager::GetIfValid();

			if (Manager)
			{
				LogOnScreen(this, "Loading monster...", FColor::Yellow);
				
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &AGRGameModeBase::OnMonsterLoaded, SelectedRow->MonsterId, Locations[0]);;

				Manager->LoadPrimaryAsset(SelectedRow->MonsterId, Bundles, Delegate);
			}
		}
	}
}

void AGRGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	LogOnScreen(this, "Monster loading finished.", FColor::Green);
	
	UAssetManager* Manager = UAssetManager::GetIfValid();

	if (Manager)
	{
		UGRMonsterData* MonsterData = Cast<UGRMonsterData>(Manager->GetPrimaryAssetObject(LoadedId));

		if (MonsterData)
		{
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);

			if (NewBot)
			{
				LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));

				// Grant special actions, buffs etc.
				UGRActionComponent* ActionComp = Cast<UGRActionComponent>(NewBot->GetComponentByClass(UGRActionComponent::StaticClass()));

				if (ActionComp)
				{
					for (TSubclassOf<UGRAction> ActionClass : MonsterData->Actions)
					{
						ActionComp->AddAction(NewBot, ActionClass);
					}
				}
			}
		}
	}
}

void AGRGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn powerup EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	// Keep used locations to easily check distance between points
	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	// Break out if we reached the desired count or if we have no more potential positions remaining
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		// Pick a random location from remaining points.
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLocation = Locations[RandomLocationIndex];
		// Remove to avoid picking again
		Locations.RemoveAt(RandomLocationIndex);

		// Check minimum distance requirement
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPowerupDistance)
			{
				// Show skipped locations due to distance
				//DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 20, FColor::Red, false, 10.0f);

				// too close, skip to next attempt
				bValidLocation = false;
				break;
			}
		}

		// Failed the distance test
		if (!bValidLocation)
		{
			continue;
		}

		// Pick a random powerup-class
		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);

		// Keep for distance checks
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}

void AGRGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		
		RestartPlayer(Controller);
	}
}

void AGRGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AGRGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	// Make sure we have assigned at least one power-up class
	if (ensure(PowerupClasses.Num() > 0))
	{
		// Run EQS to find potential power-up spawn locations
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, PowerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AGRGameModeBase::OnPowerupSpawnQueryCompleted);
		}
	}
}

void AGRGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// ??????? ????????? PlayerState, ? ??? ????? ???????? ?? ??????.) ? ??? ????? ?????????? BeginPlayingState() ?? ????????????.
	AGRPlayerState* PS = NewPlayer->GetPlayerState<AGRPlayerState>();

	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}

	// ???????... ? ????? _Implementation?
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void AGRGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));

	// Respawn Players after delay
	AGRCharacterBase* Player = Cast<AGRCharacterBase>(VictimActor);

	if (Player)
	{
		float RespawnDelay = 2.f;
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	// Get Credits for killing.
	APawn* KillerPawn = Cast<APawn>(Killer);

	if (KillerPawn)
	{
		AGRPlayerState* PS = KillerPawn->GetPlayerState<AGRPlayerState>();
		if (PS)
		{
			PS->AddCredits(CreditsPerKill);
		}
	}

}

void AGRGameModeBase::KillAll()
{
	for (TActorIterator<AGRCharacterAI> It(GetWorld()); It; ++It)
	{
		AGRCharacterAI* Bot = *It;

		UGRAttributeComponent* HealthComp = UGRAttributeComponent::GetAttributes(Bot);

		if (ensure(HealthComp) && HealthComp->IsAlive())
		{
			HealthComp->Kill(this); // Pass in player for kill credit.
		}
	}
}

void AGRGameModeBase::WriteSaveGame()
{
	// Debug
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Saving game..."));
	
	// Iterate all PlayerStates, we don`t have proper ID to match yet (required Steam or EOS)
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AGRPlayerState* PS = Cast<AGRPlayerState>(GameState->PlayerArray[i]);

		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // Single player only at this point.
		}
	}

	// Clear old data from save.
	CurrentSaveGame->SavedActors.Empty();

	// Iterate the entire world of actors
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		// Only interested in our "Gameplay Actors"
		if (!Actor->Implements<UGRGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.Name = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		// Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Arch(MemWriter, true);
		// Find only variables with UPROPERTY(SaveGame)
		Arch.ArIsSaveGame = true;
		// Converts Actor`s SaveGame UPROPERTIES into binary array
		Actor->Serialize(Arch);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	// Call SaveGameToSlot to serialize and save our SaveGameObject with name: <SaveGameSlotName>.sav
	const bool bIsSaved = UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);

	// Debug
	//if (bIsSaved)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Game Saved"));
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Game did not save..."));
	//}
}

void AGRGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UGRSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		// Debug
		//UE_LOG(LogTemp, Warning, TEXT("Loaded SaveGame Data."));

		// Load Actors Transform
		// Iterate the entire world of actors
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			// Only interested in our "Gameplay Actors"
			if (!Actor->Implements<UGRGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.Name == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Arch(MemReader, true);
					// Find only variables with UPROPERTY(SaveGame)
					Arch.ArIsSaveGame = true;
					// ConvertsByte array data back into Actor`s variables
					Actor->Serialize(Arch);

					IGRGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<UGRSaveGame>(UGameplayStatics::CreateSaveGameObject(UGRSaveGame::StaticClass()));

		UE_LOG(LogTemp, Warning, TEXT("Created new SaveGame Data."));
	}
}
