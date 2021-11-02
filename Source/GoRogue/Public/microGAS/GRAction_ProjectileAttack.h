// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "microGAS/GRAction.h"
#include "GRAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;

/**
 * 
 */
UCLASS()
class GOROGUE_API UGRAction_ProjectileAttack : public UGRAction
{
	GENERATED_BODY()
	
public:

	UGRAction_ProjectileAttack();

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim = nullptr;

	UPROPERTY(Editanywhere, Category = "Attack")
	float AttackAnimDelay = 0.2f;

	UPROPERTY(Editanywhere, Category = "Attack")
	float AttackDistance = 3000.f;

	UPROPERTY(Editanywhere, Category = "Attack")
	float TraceShapeRadius = 20.f;

	/* Particle System played during attack animation */
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

};
