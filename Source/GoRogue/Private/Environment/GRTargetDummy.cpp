// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/GRTargetDummy.h"
#include "Components/GRAttributeComponent.h"

// Sets default values
AGRTargetDummy::AGRTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	HealthComp = CreateDefaultSubobject<UGRAttributeComponent>(TEXT("HealthComp"));
	// Trigger when health is changed (damage / healing)
	HealthComp->OnHealthChanged.AddDynamic(this, &AGRTargetDummy::OnHealthChanged);
}

void AGRTargetDummy::OnHealthChanged(AActor* InstigatorActor, UGRAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.f)
	{
		MeshComp->SetScalarParameterValueOnMaterials(FName("TimeToHit"), GetWorld()->TimeSeconds);
	}
}
