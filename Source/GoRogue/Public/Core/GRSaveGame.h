// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GRSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:

	/* Identifier for which Actor this belongs to. */
	UPROPERTY()
	FString ActorName;
	
	/* For movable Actors, keep location, rotation and scale. */
	UPROPERTY()
	FTransform ActorTransform;
};

/**
 * 
 */
UCLASS()
class GOROGUE_API UGRSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
};
