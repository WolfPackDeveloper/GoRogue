// Fill out your copyright notice in the Description page of Project Settings.


#include "microGAS/GRActionComponent.h"
#include "microGAS/GRAction.h"
#include "../GoRogue.h"

#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

DECLARE_CYCLE_STAT(TEXT("StatActionByName"), STAT_StartActionByName, STATGROUP_STANFORD);

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

void UGRActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

// Called when the game starts
void UGRActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Server only
	if (GetOwner()->HasAuthority())
	{
		for (auto ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}


void UGRActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Stop all
	TArray<UGRAction*> ActionsCopy = Actions;
	for (UGRAction* Action : ActionsCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}
	
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UGRActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Debug
	//FString DebugMessage = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, DebugMessage);

	// Draw All Actions
	for (UGRAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Yellow : FColor::White;
		
		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));

		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void UGRActionComponent::AddAction(AActor* Instigator, TSubclassOf<UGRAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	// Skip for clients
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting ro AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	UGRAction* NewAction = NewObject<UGRAction>(GetOwner(), ActionClass);

	if (ensure(NewAction))
	{
		NewAction->Initialize(this);
		
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

UGRAction* UGRActionComponent::GetAction(TSubclassOf<UGRAction> ActionClass) const
{
	for (UGRAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
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
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);
	
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
			
			// Bookmark for Unreal Insights
			TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));

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
				// If is Client. Then call function on server too.
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}
				
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

bool UGRActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UGRAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UGRActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGRActionComponent, Actions);
}

