// Fill out your copyright notice in the Description page of Project Settings.


#include "microGAS/GRActionComponent.h"
#include "microGAS/GRAction.h"

// Sets default values for this component's properties
UGRActionComponent::UGRActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGRActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}


// Called every frame
void UGRActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGRActionComponent::AddAction(TSubclassOf<UGRAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UGRAction* NewAction = NewObject<UGRAction>(this, ActionClass);

	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UGRActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UGRAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UGRActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UGRAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}

