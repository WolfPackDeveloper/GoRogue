// Fill out your copyright notice in the Description page of Project Settings.


#include "microGAS/GRActionComponent.h"
#include "microGAS/GRAction.h"

// Sets default values for this component's properties
UGRActionComponent::UGRActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


void UGRActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

// Called when the game starts
void UGRActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}


// Called every frame
void UGRActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Debug
	FString DebugMessage = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, DebugMessage);
}

void UGRActionComponent::AddAction(AActor* Instigator, TSubclassOf<UGRAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UGRAction* NewAction = NewObject<UGRAction>(this, ActionClass);

	if (ensure(NewAction))
	{
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UGRActionComponent::RemoveAction(UGRAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

bool UGRActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UGRAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				// Debug
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FailedMsg);
				
				continue;
			}
			
			// If is Client. Then call function on server too.
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
			
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
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

