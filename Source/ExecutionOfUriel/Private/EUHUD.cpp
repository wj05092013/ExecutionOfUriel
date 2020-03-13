// Fill out your copyright notice in the Description page of Project Settings.


#include "EUHUD.h"
#include "Engine/Canvas.h"

void AEUHUD::DrawHUD()
{
	Super::DrawHUD();

	EUCHECK(CrossHairTex != nullptr);

	FVector2D Center(0.5f * Canvas->ClipX, 0.5f * Canvas->ClipY);
	FVector2D DrawPosition(Center.X - (0.5f * CrossHairTex->GetSurfaceWidth()), Center.Y - (0.5f * CrossHairTex->GetSurfaceHeight()));

	FCanvasTileItem TileItem(DrawPosition, CrossHairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}
