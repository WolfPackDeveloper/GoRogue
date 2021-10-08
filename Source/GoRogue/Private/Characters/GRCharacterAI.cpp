// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GRCharacterAI.h"

// Sets default values
AGRCharacterAI::AGRCharacterAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGRCharacterAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGRCharacterAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

