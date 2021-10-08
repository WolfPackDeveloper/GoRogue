// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/GRPowerup_HealthPotion.h"
#include "Components/GRAttributeComponent.h"

//#include "Components/StaticMeshComponent.h"



AGRPowerup_HealthPotion::AGRPowerup_HealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
}


void AGRPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	UGRAttributeComponent* AttributeComp = Cast<UGRAttributeComponent>(InstigatorPawn->GetComponentByClass(UGRAttributeComponent::StaticClass()));
	// Check if not already at max health
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		// Only activate if healed successfully
		if (AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax()))
		{
			HideAndCooldownPowerup();
		}
	}
}