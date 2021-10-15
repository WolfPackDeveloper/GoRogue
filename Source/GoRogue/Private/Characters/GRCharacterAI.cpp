// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GRCharacterAI.h"
#include "Components/GRAttributeComponent.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "DrawDebugHelpers.h"

AGRCharacterAI::AGRCharacterAI()
{
    HealthComp = CreateDefaultSubobject<UGRAttributeComponent>("HealthComp");

    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AGRCharacterAI::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    
    PawnSensingComp->OnSeePawn.AddDynamic(this, &AGRCharacterAI::OnPawnSeen);
    HealthComp->OnHealthChanged.AddDynamic(this, &AGRCharacterAI::OnHealthChanged);
}

void AGRCharacterAI::SetTargetActor(AActor* NewTarget)
{
    FName ValueKeyName = "TargetActor";

    AAIController* AIC = Cast<AAIController>(GetController());

    if (AIC)
    {
        AIC->GetBlackboardComponent()->SetValueAsObject(ValueKeyName, NewTarget);
    }
}

void AGRCharacterAI::OnPawnSeen(APawn* Pawn)
{
 
    SetTargetActor(Pawn);

    DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.f, true);
}

void AGRCharacterAI::OnHealthChanged(AActor* InstigatorActor, UGRAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    // Если наносится урон - начинаем мигать.
    if (Delta < 0.0f)
    {
        
        if (InstigatorActor != this)
        {
            SetTargetActor(InstigatorActor);
        }

        if (NewHealth <= 0.f)
        {
            // Stop BT
            AAIController* AIC = Cast<AAIController>(GetController());
            if (AIC)
            {
                AIC->GetBrainComponent()->StopLogic("Killed");
            }

            // Ragdoll
            GetMesh()->SetAllBodiesSimulatePhysics(true);
            GetMesh()->SetCollisionProfileName("Ragdoll");

            // Lifespan
            SetLifeSpan(10.f);
        }
    }
}
