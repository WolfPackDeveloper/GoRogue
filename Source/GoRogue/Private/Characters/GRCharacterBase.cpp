// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GRCharacterBase.h"
#include "Components/GRAttributeComponent.h"
#include "Components/GRInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
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

	HealthComp = CreateDefaultSubobject<UGRAttributeComponent>(TEXT("HealthComp"));
	InteractionComp = CreateDefaultSubobject<UGRInteractionComponent>(TEXT("InteractionComp"));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

FVector AGRCharacterBase::GetAimLocation()
{
	FVector AimLocation = FVector::ZeroVector;
	
	const APlayerController* PlayerController = GetController<APlayerController>();

	if (!ensure(PlayerController))
	{
		return AimLocation;
	}

	FVector ViewLocation;
	FRotator ViewRotation;

	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

	FVector TraceStart = ViewLocation;
	FVector ShotDirection = ViewRotation.Vector();
	FVector TraceEnd = TraceStart + ShotDirection * ShotDistance;

	// "Калибровка" прицела в зависимости от наличия попадания трасировке пути снаряда.
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

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

// Called when the game starts or when spawned
void AGRCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
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

void AGRCharacterBase::PrimaryAttack()
{
	float DelayTime = 0.2f;

	PlayAnimMontage(AttackAnim);
	
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AGRCharacterBase::PrimaryAttack_TimeElapsed, DelayTime);
}

void AGRCharacterBase::PrimaryAttack_TimeElapsed()
{
	if (!ensure(ProjectileClass))
	{
		return;
	}
	
	FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));
	FVector AimLocation = GetAimLocation();
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, AimLocation);
	FTransform SpawnTM = FTransform(SpawnRotation, SpawnLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void AGRCharacterBase::PrimaryInterract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInterract();
	}
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
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AGRCharacterBase::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AGRCharacterBase::PrimaryInterract);

}

