// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GRCharacterBase.generated.h"

class UAnimMontage;
class USpringArmComponent;
class UCameraComponent;

class UGRAttributeComponent;
class UGRInteractionComponent;

UCLASS()
class GOROGUE_API AGRCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGRCharacterBase();

private:

	// RangeAttack support function
	FVector GetAimLocation();

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

protected:

	// === Attack ===

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Attack")
	float ShotDistance = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	FTimerHandle TimerHandle_PrimaryAttack;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void PrimaryInterract();
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
