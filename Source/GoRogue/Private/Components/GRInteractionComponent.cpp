// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GRInteractionComponent.h"
#include "Interfaces/GRGameplayInterface.h"

#include "Kismet/KismetSystemLibrary.h" //Trace

static TAutoConsoleVariable<bool> CVarDrawDebugInteraction(TEXT("su.InteractionDrawDebug"), false, TEXT("Enable Debug Lines for Interact Components."), ECVF_Cheat);

// Sets default values for this component's properties
UGRInteractionComponent::UGRInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGRInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGRInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGRInteractionComponent::PrimaryInterract()
{
	bool bDrawDebug = CVarDrawDebugInteraction.GetValueOnGameThread();
	float SightLength = 1000.f;
	float ShapeSize = 30.f;
	float Duration = 2.f;
	float Thickness = 2.f;
	int ShapeSegmentsNum = 32;


	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	AActor* Owner = GetOwner();
	
	FVector CharEyeLocation;
	FRotator CharEyeRotation;

	Owner->GetActorEyesViewPoint(CharEyeLocation, CharEyeRotation);

	FVector TraceEnd = CharEyeLocation + (CharEyeRotation.Vector() * SightLength);

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(ShapeSize);

	bool bHit = GetWorld()->SweepMultiByObjectType(
		Hits,
		CharEyeLocation,
		TraceEnd,
		FQuat::Identity,
		ObjectQueryParams,
		Shape
	);

	FColor LineColor = bHit ? FColor::Green : FColor::Red;

	for (FHitResult Hit : Hits)
	{
		if (bDrawDebug)
		{
			UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Hit.ImpactPoint, ShapeSize, ShapeSegmentsNum, LineColor, Duration, Thickness);
		}
		
		AActor* HitActor = Hit.GetActor();

		if (HitActor)
		{
			if (HitActor->Implements<UGRGameplayInterface>())
			{
				APawn* OwnerPawn = Cast<APawn>(Owner);

				IGRGameplayInterface::Execute_Interact(HitActor, OwnerPawn);
				break;
			}
		}
	}

	if (bDrawDebug)
	{
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), CharEyeLocation, TraceEnd, LineColor, Duration, Thickness);
	}
}

