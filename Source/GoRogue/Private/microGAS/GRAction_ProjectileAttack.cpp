// Fill out your copyright notice in the Description page of Project Settings.


#include "microGAS/GRAction_ProjectileAttack.h"

#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h" // Emit particle system
#include "Kismet/KismetMathLibrary.h" // FindLookAtRotation

UGRAction_ProjectileAttack::UGRAction_ProjectileAttack()
{
	// Was set in header.
	//AttackDistance = 3000.f;
	//AttackAnimDelay = 0.2f;
	HandSocketName = "Muzzle_01";
}


FVector UGRAction_ProjectileAttack::GetAimLocation(ACharacter* InstigatorCharacter)
{
	FVector AimLocation = FVector::ZeroVector;

	const APlayerController* PlayerController = InstigatorCharacter->GetController<APlayerController>();

	if (!ensure(PlayerController))
	{
		return AimLocation;
	}

	FVector ViewLocation;
	FRotator ViewRotation;

	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

	FVector TraceStart = ViewLocation;
	FVector ShotDirection = ViewRotation.Vector();
	FVector TraceEnd = TraceStart + ShotDirection * AttackDistance;

	// "Калибровка" прицела в зависимости от наличия попадания трасировке пути снаряда.
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(InstigatorCharacter);

	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		CollisionParams
	);

	if (bHit)
	{
		AimLocation = HitResult.ImpactPoint;
	}
	else
	{
		AimLocation = TraceEnd;
	}

	return AimLocation;
}

void UGRAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (!ensureAlways(ProjectileClass))
	{
		return;
	}

	FVector SpawnLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
	FVector AimLocation = GetAimLocation(InstigatorCharacter);
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, AimLocation);
	FTransform SpawnTM = FTransform(SpawnRotation, SpawnLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = InstigatorCharacter;
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

	StopAction(InstigatorCharacter);
}

void UGRAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);

	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);
		// Casting Effect Plaing
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
}

