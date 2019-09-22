// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGun.h"

// Engine includes
#include "Components/ArrowComponent.h"

ABaseGun::ABaseGun():
	MuzzleLocation(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a gun mesh component
	MuzzleLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->ArrowColor = FColor::Cyan;
	MuzzleLocation->bIsScreenSizeScaled = true;
	MuzzleLocation->SetupAttachment(RootComponent);
}
