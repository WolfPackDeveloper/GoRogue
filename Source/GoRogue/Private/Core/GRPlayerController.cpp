// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GRPlayerController.h"

void AGRPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}
