// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HalfLife2PlaygroundHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

AHalfLife2PlaygroundHUD::AHalfLife2PlaygroundHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPersonAssets/Textures/Crosshair"));
	if (CrosshairTexObj.Object != nullptr)
	{
		CrosshairTex = CrosshairTexObj.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Crosshair texture was not found!"));
	}
}

void AHalfLife2PlaygroundHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X - CrosshairTex->GetSurfaceWidth() / 2), (Center.Y - CrosshairTex->GetSurfaceHeight() / 2));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}
