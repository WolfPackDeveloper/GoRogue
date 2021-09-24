// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GRCharacterBase.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
AGRCharacterBase::AGRCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void AGRCharacterBase::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AGRCharacterBase::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

//void AGRCharacterBase::LookUp(float Value)
//{
//
//}
//
//void AGRCharacterBase::TurnRight(float Value)
//{
//
//}


// Called when the game starts or when spawned
void AGRCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
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

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerRollInput);
	PlayerInputComponent->BindAxis("TurnRight", this, &APawn::AddControllerYawInput);

}

