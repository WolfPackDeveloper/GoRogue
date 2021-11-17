// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GRMagicProjectile.h"
#include "Components/GRAttributeComponent.h"
#include "Core/GRGameplayFunctionLibrary.h"
#include "microGAS/GRActionComponent.h"
#include "microGAS/GRActionEffect.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AGRMagicProjectile::AGRMagicProjectile()
{
	SphereComp->SetSphereRadius(DefaultSphereRadius);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AGRMagicProjectile::OnActorOverlap);
}

void AGRMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (OtherActor && OtherActor != GetInstigator())
	{
		UGRActionComponent* ActionComp = Cast<UGRActionComponent>(OtherActor->GetComponentByClass(UGRActionComponent::StaticClass()));

		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MoveComp->Velocity = -MoveComp->Velocity;

			// при парировании меняем "владельца" снаряда, чтобы он мог нанести урон тому, кто его выпустил.
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		// Apply Damage & Impulse
		if (UGRGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			Explode();

			if (ActionComp && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
		}
	}
}

