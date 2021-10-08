// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GRCharacterBase.generated.h"

class UAnimMontage;
class UCameraComponent;
class USpringArmComponent;
class UParticleSystem;

class UGRAttributeComponent;
class UGRInteractionComponent;

UCLASS()
class GOROGUE_API AGRCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGRCharacterBase();

	virtual void PostInitializeComponents() override;

private:

	// RangeAttack support function
	FVector GetAimLocation();

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

protected:
	
	// === Components ===

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	UGRInteractionComponent* InteractionComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UGRAttributeComponent* HealthComp;

	// === Attack ===

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Attack")
	float ShotDistance = 3000.f;

	UPROPERTY(Editanywhere, Category = "Attack")
	float AttackAnimDelay = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim = nullptr;

	/* Particle System played during attack animation */
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_BlackholeAttack;
	FTimerHandle TimerHandle_Dash;

	// === Effects ===

	/* VisibleAnywhere = read-only, still useful to view in-editor and enforce a convention. */
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName = "TimeToHit";

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName = "Muzzle_01";

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	
	void MoveRight(float Value);
	
	void PrimaryInterract();
	
	void PrimaryAttack();
	
	void PrimaryAttack_TimeElapsed();

	void BlackHoleAttack();

	void BlackholeAttack_TimeElapsed();

	void Dash();

	void Dash_TimeElapsed();

	void StartAttackEffects();

public:	
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UGRAttributeComponent* OwningComp, float NewHealth, float Delta);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
