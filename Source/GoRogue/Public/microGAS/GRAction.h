// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "GRAction.generated.h"

class UGRActionComponent;
class UWorld;

/**
 * 
 */
UCLASS(Blueprintable)
class GOROGUE_API UGRAction : public UObject
{
	GENERATED_BODY()

protected:

	bool bIsRunning = false;

	/* Tags added to owning actor when activated, and removed, when action stops. */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/* Action can only start if OwningActor has none of these Tags applied. */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UGRActionComponent* GetOwningComponent() const;

public:

	/* Action nickname to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	/* Start immediatly when added to action component */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);
	virtual bool CanStart_Implementation(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);
	virtual void StartAction_Implementation(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StopAction(AActor* Instigator);
	virtual void StopAction_Implementation(AActor* Instigator);

	UWorld* GetWorld() const override;
};
