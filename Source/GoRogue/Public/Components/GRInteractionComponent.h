// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GRInteractionComponent.generated.h"

class UGRWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOROGUE_API UGRInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UGRInteractionComponent();

private:

	APawn* MyPawn;

protected:

	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_WorldDynamic;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UGRWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	UGRWorldUserWidget* DefaultWidgetInstance;

	// Reliable - Will always arrive, eventually. Request will be re-sent unless an acknowledgment was receieved.
	// Unreliable - Not guaranteed, packet can get lost and won`t retry.

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

	void FindBestInteractable();
	
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PrimaryInterract();
		
};
