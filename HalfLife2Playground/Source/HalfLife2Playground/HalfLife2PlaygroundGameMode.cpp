// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HalfLife2PlaygroundGameMode.h"
#include "HalfLife2PlaygroundHUD.h"
#include "HalfLife2PlaygroundCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHalfLife2PlaygroundGameMode::AHalfLife2PlaygroundGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Core/Characters/FirstPersonCharacter"));
	if (PlayerPawnClassFinder.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FPCharacter BP was not found!"));
	}
	// use our custom HUD class
	HUDClass = AHalfLife2PlaygroundHUD::StaticClass();
}
