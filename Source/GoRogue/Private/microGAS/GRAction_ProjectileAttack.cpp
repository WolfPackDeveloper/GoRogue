// Fill out your copyright notice in the Description page of Project Settings.


#include "microGAS/GRAction_ProjectileAttack.h"

#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h" // Emit particle system
#include "Kismet/KismetMathLibrary.h" // FindLookAtRotation

UGRAction_ProjectileAttack::UGRAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
}

void UGRAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocarion = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(TraceShapeRadius);

		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * AttackDistance);

		FHitResult Hit;
		bool bHit = GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, TraceParams);

		if (bHit)
		{
			TraceEnd = Hit.ImpactPoint;
		}

		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocarion).Rotator();
		FTransform SpawnTransform = FTransform(ProjRotation, HandLocarion);

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}

void UGRAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);

	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);
		// Casting Effect Playing
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		// В примере это работает ок. У меня же не реплицируется созданный снаряд на клиента.
		// Возможно нужно пересмотреть ещё раз уроки по сети... Но по файлам всё сходится... Странно.
		// P.S. У меня задвоения снаряда не было...
		// P.P.S. Потому что ты мудак - у тебя было закомменченов базовом классе снаряда, в конструкторе функция SetReplicate(true). Что это такое  - сам догадаешься.
		if (Character->HasAuthority())
		{
			FTimerHandle TimerHandle_AttackDelay;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
		}
	}
}

