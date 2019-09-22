// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HalfLife2PlaygroundHUD.generated.h"

UCLASS()
class AHalfLife2PlaygroundHUD : public AHUD
{
	GENERATED_BODY()

public:

	AHalfLife2PlaygroundHUD();

	/** Primary draw call for the HUD 
	*/
	virtual void DrawHUD() override;

private:

	/** Crosshair asset pointer.
	*/
	UPROPERTY()
	class UTexture2D* CrosshairTex;

};

