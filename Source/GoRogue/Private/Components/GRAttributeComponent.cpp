// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GRAttributeComponent.h"

// Sets default values for this component's properties
UGRAttributeComponent::UGRAttributeComponent()
{
	Health = HealthMax;
}

UGRAttributeComponent* UGRAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UGRAttributeComponent>(FromActor->GetComponentByClass(UGRAttributeComponent::StaticClass()));
	}
	
	return nullptr;
}

bool UGRAttributeComponent::IsActorAlive(AActor* Actor)
{
	UGRAttributeComponent* HealthComp = GetAttributes(Actor);
	
	if (HealthComp)
	{
		return HealthComp->IsAlive();
	}
	
	return false;
}

bool UGRAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

bool UGRAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float UGRAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool UGRAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	//Health += Delta;
	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.f, HealthMax);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	return ActualDelta != 0.f;
}
