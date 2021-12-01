// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GRAnimInstance.h"
#include "microGAS/GRActionComponent.h"

#include "GameplayTagContainer.h"

void UGRAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();

	if (OwningActor)
	{
		ActionComp = Cast<UGRActionComponent>(OwningActor->GetComponentByClass(UGRActionComponent::StaticClass()));
	}
}

void UGRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");

	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
