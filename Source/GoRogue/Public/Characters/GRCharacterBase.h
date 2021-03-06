// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GRCharacterBase.generated.h"

class UAnimMontage;
class UCameraComponent;
class USpringArmComponent;
class UParticleSystem;

class UGRActionComponent;
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

protected:
	
	// === Components ===

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	UGRInteractionComponent* InteractionComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UGRAttributeComponent* AttributeComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UGRActionComponent* ActionComp = nullptr;

	/* VisibleAnywhere = read-only, still useful to view in-editor and enforce a convention. */
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName = "TimeToHit";

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	
	void MoveRight(float Value);
	
	void SprintStart();

	void SprintStop();

	void PrimaryInterract();
	
	void PrimaryAttack();
	
	void BlackHoleAttack();

	void Dash();

	virtual FVector GetPawnViewLocation() const override;

public:	
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UGRAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
