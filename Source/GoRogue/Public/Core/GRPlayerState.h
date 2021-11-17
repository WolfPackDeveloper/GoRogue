// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GRPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AGRPlayerState*, PlayerState, int32, NewCredits, int32, Delta);

class UGRSaveGame;

/**
 * 
 */
UCLASS()
class GOROGUE_API AGRPlayerState : public APlayerState
{
	GENERATED_BODY()
	

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 Credits = 0;

public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits(int32 Delta) const	;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(UGRSaveGame* SaveObject);
	virtual void SavePlayerState_Implementation(UGRSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(UGRSaveGame* SaveObject);
	virtual void LoadPlayerState_Implementation(UGRSaveGame* SaveObject);

};
