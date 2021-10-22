// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GRBTTask_RangedAttack.h"
#include "Components/GRAttributeComponent.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UGRBTTask_RangedAttack::UGRBTTask_RangedAttack()
{

}

EBTNodeResult::Type UGRBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	FName MuzzleSocketName = "Muzzle_01";
	FName TargetActorValName = "TargetActor";

	AAIController* MyController = OwnerComp.GetAIOwner();
	
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		
		if (MyPawn == nullptr)
		{
			EBTNodeResult::Failed;
		}

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorValName));
		if (TargetActor == nullptr)
		{
			EBTNodeResult::Failed;
		}

		if (!UGRAttributeComponent::IsActorAlive(TargetActor))
		{
			EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation(MuzzleSocketName);
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = MyPawn;

		AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);

		return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
