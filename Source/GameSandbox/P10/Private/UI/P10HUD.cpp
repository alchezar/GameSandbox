// Copyright (C) 2023, IKinder

#include "P10/Public/UI/P10HUD.h"

#include "Engine/Canvas.h"

AP10HUD::AP10HUD()
{
	
}

void AP10HUD::DrawHUD()
{
	Super::DrawHUD();

	const FVector2D Center = FVector2D(Canvas->ClipX / 2.f, Canvas->ClipY / 2.f);
	const FVector2D CrosshairPosition = FVector2D(Center.X - CrosshairTex->GetSizeX() / 2.f, Center.Y - CrosshairTex->GetSizeY() / 2.f);
	FCanvasTileItem TileItem = FCanvasTileItem(CrosshairPosition, CrosshairTex->GetResource(), FLinearColor::White);
	TileItem.BlendMode = ESimpleElementBlendMode::SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}
