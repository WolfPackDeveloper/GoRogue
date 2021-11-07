// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GRAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


void AGRAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Assign BegaviorTree it your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
