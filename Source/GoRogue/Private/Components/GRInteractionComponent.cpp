// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GRInteractionComponent.h"
#include "Core\GRWorldUserWidget.h"
#include "Interfaces/GRGameplayInterface.h"

#include "Kismet/KismetSystemLibrary.h" //Trace

static TAutoConsoleVariable<bool> CVarDrawDebugInteraction(TEXT("su.InteractionDrawDebug"), false, TEXT("Enable Debug Lines for Interact Components."), ECVF_Cheat);

// Sets default values for this component's properties
UGRInteractionComponent::UGRInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UGRInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	IGRGameplayInterface::Execute_Interact(InFocus, OwnerPawn);
}

void UGRInteractionComponent::FindBestInteractable()
{
	bool bDrawDebug = CVarDrawDebugInteraction.GetValueOnGameThread();

	// Debug properties
	float Duration = 2.f;
	float Thickness = 2.f;
	int ShapeSegmentsNum = 32;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* Owner = GetOwner();

	FVector CharEyeLocation;
	FRotator CharEyeRotation;

	Owner->GetActorEyesViewPoint(CharEyeLocation, CharEyeRotation);

	FVector TraceEnd = CharEyeLocation + (CharEyeRotation.Vector() * TraceDistance);

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bHit = GetWorld()->SweepMultiByObjectType(
		Hits,
		CharEyeLocation,
		TraceEnd,
		FQuat::Identity,
		ObjectQueryParams,
		Shape
	);

	FColor LineColor = bHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		if (bDrawDebug)
		{
			UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, ShapeSegmentsNum, LineColor, Duration, Thickness);
		}

		AActor* HitActor = Hit.GetActor();

		if (HitActor)
		{
			if (HitActor->Implements<UGRGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UGRWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}
		
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}

	}

	if (bDrawDebug)
	{
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), CharEyeLocation, TraceEnd, LineColor, Duration, Thickness);
	}
}

void UGRInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	// Кастуем тут, а не в тике. Но в туторе было в тике. Держи это в уме.
	MyPawn = Cast<APawn>(GetOwner());
}

void UGRInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MyPawn)
	{
		if (MyPawn->IsLocallyControlled())
		{
			FindBestInteractable();
		}
	}
}

void UGRInteractionComponent::PrimaryInterract()
{
	ServerInteract(FocusedActor);
}

