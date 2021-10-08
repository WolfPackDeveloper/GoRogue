// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "Weapon/GRProjectileBase.h"
#include "GRMagicProjectile.generated.h"

//class USphereComponent;
//class UProjectileMovementComponent;
//class UParticleSystemComponent;

UCLASS()
class GOROGUE_API AGRMagicProjectile : public AGRProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGRMagicProjectile();

private:

	float DefaultSphereRadius = 20.f;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float DamageAmount = 20.f;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//USphereComponent* SphereComp = nullptr;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UProjectileMovementComponent* MovementComp = nullptr;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UParticleSystemComponent* EffectComp = nullptr;
	
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
