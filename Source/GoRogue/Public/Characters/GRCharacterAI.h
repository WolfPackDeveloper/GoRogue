// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GRCharacterAI.generated.h"

class UGRAttributeComponent;
class UGRActionComponent;
class UGRWorldUserWidget;

class UPawnSensingComponent;

UCLASS()
class GOROGUE_API AGRCharacterAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGRCharacterAI();

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UGRWorldUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UGRWorldUserWidget> SpottedWidgetClass;

	UPROPERTY()
	UGRWorldUserWidget* ActiveHealthBar = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UGRAttributeComponent* AttributeComp = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UGRActionComponent* ActionComp = nullptr;

	/* VisibleAnywhere = read-only, still useful to view in-editor and enforce a convention. */
	/* Material parameter for Hitflashes */
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName = "TimeToHit";

	/* Key for AI Blackboard 'TargetActor' */
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TargetActorKey = "TargetActor";

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPawnSeen();
	void MulticastPawnSeen_Implementation();

public:

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UGRAttributeComponent* OwningComp, float NewHealth, float Delta);

};
