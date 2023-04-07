// Copyright (C) 2023, IKinder

#include "STU_GameHUD.h"
#include "Engine/Canvas.h"

ASTU_GameHUD::ASTU_GameHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASTU_GameHUD::DrawHUD()
{
	Super::DrawHUD();
	DrawCrossHair();
}

void ASTU_GameHUD::DrawCrossHair()
{
	constexpr float        HalfLineSize  = 10.f;
	constexpr float        LineThickness = 2.f;
	const TInterval<float> Center(Canvas->SizeX / 2, Canvas->SizeY / 2);
	const FLinearColor     LineColor = FLinearColor::Red;
	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}
