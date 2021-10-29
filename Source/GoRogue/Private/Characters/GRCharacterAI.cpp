// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GRCharacterAI.h"
#include "Components/GRAttributeComponent.h"
#include "Core/GRWorldUserWidget.h"
#include "microGAS/GRActionComponent.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "DrawDebugHelpers.h"

AGRCharacterAI::AGRCharacterAI()
{
    ActionComp = CreateDefaultSubobject<UGRActionComponent>(TEXT("ActionComp"));
    
    HealthComp = CreateDefaultSubobject<UGRAttributeComponent>(TEXT("HealthComp"));

    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    //GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
    //GetMesh()->SetGenerateOverlapEvents(true);
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

    //DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.f, true);
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


        if (ActiveHealthBar == nullptr)
        {
			ActiveHealthBar = CreateWidget<UGRWorldUserWidget>(GetWorld(), HealthBarWidgetClass);

			if (ActiveHealthBar)
			{
                ActiveHealthBar->AttachedActor = this;
                ActiveHealthBar->AddToViewport();
			}
        }

        GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

        // Died
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
            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            GetCharacterMovement()->DisableMovement();

            // Lifespan
            SetLifeSpan(10.f);
        }
    }
}
