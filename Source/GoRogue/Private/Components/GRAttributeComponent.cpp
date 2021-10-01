// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GRAttributeComponent.h"

// Sets default values for this component's properties
UGRAttributeComponent::UGRAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

bool UGRAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}
