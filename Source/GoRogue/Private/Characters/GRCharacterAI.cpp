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
    
    AttributeComp = CreateDefaultSubobject<UGRAttributeComponent>(TEXT("HealthComp"));

    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

    // Ensures we receive a controlled when spawned in the level by our gamemode
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Disabled on capsule to let projectiles pass through capsule and hit mesh instead
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	// Enabled on mesh to react to incoming projectiles
	GetMesh()->SetGenerateOverlapEvents(true);

    // Set in .h file
	//TimeToHitParamName = "TimeToHit";
	//TargetActorKey = "TargetActor";
}

void AGRCharacterAI::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    
    PawnSensingComp->OnSeePawn.AddDynamic(this, &AGRCharacterAI::OnPawnSeen);
    AttributeComp->OnHealthChanged.AddDynamic(this, &AGRCharacterAI::OnHealthChanged);
}

void AGRCharacterAI::SetTargetActor(AActor* NewTarget)
{
    AAIController* AIC = Cast<AAIController>(GetController());

    if (AIC)
    {
        AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
    }
}

AActor* AGRCharacterAI::GetTargetActor() const
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}

	return nullptr;
}

void AGRCharacterAI::OnPawnSeen(APawn* Pawn)
{
 	// Ignore if target already set
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);

        MulticastPawnSeen();
	}
	//DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 0.5f, true);
}

void AGRCharacterAI::MulticastPawnSeen_Implementation()
{
	UGRWorldUserWidget* NewWidget = CreateWidget<UGRWorldUserWidget>(GetWorld(), SpottedWidgetClass);
	if (NewWidget)
	{
		NewWidget->AttachedActor = this;
		// Index of 10 (or anything higher than default of 0) places this on top of any other widget.
		// May end up behind the minion health bar otherwise.
		NewWidget->AddToViewport(10);
	}
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
