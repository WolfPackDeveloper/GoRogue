// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GRGameplayFunctionLibrary.h"
#include "Components/GRAttributeComponent.h"

bool UGRGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UGRAttributeComponent* HealthComp = UGRAttributeComponent::GetAttributes(TargetActor);

	if (HealthComp)
	{
		return HealthComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	}

	return false;
}

bool UGRGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	float ImpulseForce = 300000.f;
	
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();

		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * ImpulseForce, HitResult.ImpactPoint, HitResult.BoneName);
		}

		return true;
	}
	
	return false;
}
