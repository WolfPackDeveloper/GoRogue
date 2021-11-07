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

	UPROPERTY()
	UGRWorldUserWidget* ActiveHealthBar = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UGRAttributeComponent* AttributeComp = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UGRActionComponent* ActionComp = nullptr;

	/* VisibleAnywhere = read-only, still useful to view in-editor and enforce a convention. */
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName = "TimeToHit";

	virtual void PostInitializeComponents() override;

	void SetTargetActor(AActor* NewTarget);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

public:

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UGRAttributeComponent* OwningComp, float NewHealth, float Delta);

};
