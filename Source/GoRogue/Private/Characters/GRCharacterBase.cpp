// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GRCharacterBase.h"
#include "Components/GRAttributeComponent.h"
#include "Components/GRInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "microGAS/GRActionComponent.h"

#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h" // FindLookAtRotation


// Sets default values
AGRCharacterBase::AGRCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	ActionComp = CreateDefaultSubobject<UGRActionComponent>(TEXT("ActionComp"));
	AttributeComp = CreateDefaultSubobject<UGRAttributeComponent>(TEXT("HealthComp"));
	InteractionComp = CreateDefaultSubobject<UGRInteractionComponent>(TEXT("InteractionComp"));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}


void AGRCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &AGRCharacterBase::OnHealthChanged);
}

// Called when the game starts or when spawned
void AGRCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Update UI Healthbar Value
	//AttributeComp->ApplyHealthChange(this, 0.f);
	
}

void AGRCharacterBase::MoveForward(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	AddMovementInput(ControlRotation.Vector(), Value);
}

void AGRCharacterBase::MoveRight(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void AGRCharacterBase::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void AGRCharacterBase::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void AGRCharacterBase::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void AGRCharacterBase::BlackHoleAttack()
{
	ActionComp->StartActionByName(this, "BlackHole");
}

void AGRCharacterBase::Dash()
{
	ActionComp->StartActionByName(this, "Dash");
}

FVector AGRCharacterBase::GetPawnViewLocation() const
{
	return Camera->GetComponentLocation();
}

void AGRCharacterBase::PrimaryInterract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInterract();
	}
}

void AGRCharacterBase::OnHealthChanged(AActor* InstigatorActor, UGRAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// Если наносится урон - начинаем мигать.
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		// Rage added equal to damage received (Abs to turn into positive rage number)
		float RageDelta = FMath::Abs(Delta);
		AttributeComp->ApplyRage(InstigatorActor, RageDelta);
	}
	// Время умирать.
	if (NewHealth <= 0.f && Delta < 0.f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		
		DisableInput(PlayerController);

		SetLifeSpan(5.0f);
	}
}

void AGRCharacterBase::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount /* = 100.f*/);
}

// Called every frame
void AGRCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGRCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGRCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGRCharacterBase::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRight", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AGRCharacterBase::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AGRCharacterBase::SprintStop);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AGRCharacterBase::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &AGRCharacterBase::BlackHoleAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AGRCharacterBase::Dash);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AGRCharacterBase::PrimaryInterract);
}

