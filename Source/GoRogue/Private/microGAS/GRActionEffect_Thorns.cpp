// Fill out your copyright notice in the Description page of Project Settings.


#include "microGAS/GRActionEffect_Thorns.h"
#include "microGAS/GRActionComponent.h"
#include "Components/GRAttributeComponent.h"
#include "Core/GRGameplayFunctionLibrary.h"


UGRActionEffect_Thorns::UGRActionEffect_Thorns()
{
	Duration = 0.f;
	Period = 0.f;
}

void UGRActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UGRAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	// Damage Only
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		// Round to nearest to avoid 'ugly' damage numbers and tiny reflections
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}

		// Flip to positive, so we don't end up healing ourselves when passed into damage
		ReflectedAmount = FMath::Abs(ReflectedAmount);

		// Return damage sender...
		UGRGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}

void UGRActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// Start listening
	UGRAttributeComponent* Attributes = UGRAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &UGRActionEffect_Thorns::OnHealthChanged);
	}
}

void UGRActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	// Stop listening
	UGRAttributeComponent* Attributes = UGRAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &UGRActionEffect_Thorns::OnHealthChanged);
	}
}

