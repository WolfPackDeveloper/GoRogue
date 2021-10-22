// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GRAttributeComponent.h"
#include "Core/GRGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

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

void UGRAttributeComponent::Kill(AActor* InstigatorActor)
{
	ApplyHealthChange(InstigatorActor, -GetHealthMax());
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


float UGRAttributeComponent::GetHealth() const
{
	return Health;
}

float UGRAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool UGRAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.f)
	{
		return false;
	}
	
	if (Delta < 0.f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.f, HealthMax);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	// Died
	if (ActualDelta < 0.f && Health == 0.f)
	{
		AGRGameModeBase* GM = Cast<AGRGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return ActualDelta != 0.f;
}
