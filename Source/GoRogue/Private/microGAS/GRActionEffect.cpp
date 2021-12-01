// Fill out your copyright notice in the Description page of Project Settings.


#include "microGAS/GRActionEffect.h"
#include "microGAS/GRActionComponent.h"

#include "GameFramework/GameStateBase.h"

UGRActionEffect::UGRActionEffect()
{
	bAutoStart = true;
}

void UGRActionEffect::ExecutePeriodEffect_Implementation(AActor* Instigator)
{

}

void UGRActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void UGRActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	UGRActionComponent* Comp = GetOwningComponent();

	if (Comp)
	{
		Comp->RemoveAction(this);
	}
}

float UGRActionEffect::GetTimeRemaining() const
{
	AGameStateBase* GS =  GetWorld()->GetGameState<AGameStateBase>();

	if (GS)
	{
		float EndTime = TimeStarted + Duration;

		return EndTime - GS->GetServerWorldTimeSeconds();
	}

	return Duration;
}

