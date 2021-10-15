// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GRMagicProjectile.h"
#include "Components/GRAttributeComponent.h"

#include "Components/SphereComponent.h"
//#include "GameFramework/ProjectileMovementComponent.h"
//#include "Particles/ParticleSystemComponent.h"

// Sets default values
AGRMagicProjectile::AGRMagicProjectile()
{
// 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//
//	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
//	SphereComp->SetCollisionProfileName(TEXT("Projectile"));
//	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AGRMagicProjectile::OnActorOverlap);
//	RootComponent = SphereComp;
// 
	SphereComp->SetSphereRadius(DefaultSphereRadius);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AGRMagicProjectile::OnActorOverlap);

//	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectComp"));
//	EffectComp->SetupAttachment(SphereComp);
//
//	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
//	MovementComp->InitialSpeed = 1000.f;
//	MovementComp->bRotationFollowsVelocity = true;
//	MovementComp->bInitialVelocityInLocalSpace = true;
}

// Called when the game starts or when spawned
//void AGRMagicProjectile::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}

void AGRMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetInstigator())
	{
		return;
	}
	
	UGRAttributeComponent* AttributeComp = Cast<UGRAttributeComponent>(OtherActor->GetComponentByClass(UGRAttributeComponent::StaticClass()));
	if (AttributeComp)
	{
		// minus in front of DamageAmount to apply the change as damage, not healing
		AttributeComp->ApplyHealthChange(GetInstigator(), -DamageAmount);

		//Destroy();

		// Only explode when we hit something valid
		Explode();
	}
}

// Called every frame
//void AGRMagicProjectile::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

