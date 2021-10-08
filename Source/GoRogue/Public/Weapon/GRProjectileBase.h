// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GRProjectileBase.generated.h"

class UAudioComponent;
class UMatineeCameraShake;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class USoundCue;
class USphereComponent;

UCLASS(ABSTRACT) // 'ABSTRACT' marks this class as incomplete, keeping this out of certain dropdowns windows like SpawnActor in Unreal Editor.
class GOROGUE_API AGRProjectileBase : public AActor
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	AGRProjectileBase();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	TSubclassOf<UMatineeCameraShake> ImpactShake;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeInnerRadius = 250.f;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeOuterRadius = 2500.f;
		
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundCue* ImpactSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MoveComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* EffectComp = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent* AudioComp = nullptr;


	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	virtual void Explode_Implementation();

	virtual void PostInitializeComponents() override;

};
