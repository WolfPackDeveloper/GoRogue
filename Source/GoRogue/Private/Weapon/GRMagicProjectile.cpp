// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GRMagicProjectile.h"
#include "Components/GRAttributeComponent.h"
#include "Core/GRGameplayFunctionLibrary.h"

#include "Components/SphereComponent.h"

// Sets default values
AGRMagicProjectile::AGRMagicProjectile()
{
	SphereComp->SetSphereRadius(DefaultSphereRadius);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AGRMagicProjectile::OnActorOverlap);
}

void AGRMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetInstigator())
	{
		return;
	}

	if (UGRGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
	{
		Explode();
	}
	
	//UGRAttributeComponent* AttributeComp = Cast<UGRAttributeComponent>(OtherActor->GetComponentByClass(UGRAttributeComponent::StaticClass()));
	//if (AttributeComp)
	//{
	//	// minus in front of DamageAmount to apply the change as damage, not healing
	//	AttributeComp->ApplyHealthChange(GetInstigator(), -DamageAmount);
	//	// Only explode when we hit something valid
	//	Explode();
	//}
}

