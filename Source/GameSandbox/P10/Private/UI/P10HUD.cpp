// Copyright (C) 2023, IKinder

#include "P10/Public/UI/P10HUD.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"

AP10HUD::AP10HUD(): CrosshairTex(nullptr), CrosshairWidget(nullptr), HealthIndicator(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AP10HUD::BeginPlay()
{
	Super::BeginPlay();
	check(CrosshairWidgetClass)
	check(MissionCompletedWidgetClass)
	check(HealthIndicatorWidgetClass)

	DrawCrosshair(true);
	DrawHealthIndicator();
}

void AP10HUD::DrawHUD()
{
	Super::DrawHUD();

	if (CrosshairTex)
	{
		const FVector2D Center = FVector2D(Canvas->ClipX / 2.f, Canvas->ClipY / 2.f);
		const FVector2D CrosshairPosition = FVector2D(Center.X - CrosshairTex->GetSizeX() / 2.f, Center.Y - CrosshairTex->GetSizeY() / 2.f);
		FCanvasTileItem TileItem = FCanvasTileItem(CrosshairPosition, CrosshairTex->GetResource(), FLinearColor::White);
		TileItem.BlendMode = ESimpleElementBlendMode::SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}

void AP10HUD::DrawCrosshair(const bool bDraw)
{
	if (!CrosshairWidget)
	{
		CrosshairWidget = CreateWidget(GetWorld(), CrosshairWidgetClass);
		check(CrosshairWidget)
		CrosshairWidget->AddToViewport();
	}
	CrosshairWidget->SetVisibility(bDraw ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void AP10HUD::DrawHealthIndicator()
{
	HealthIndicator = CreateWidget(GetWorld(), HealthIndicatorWidgetClass);
	check(HealthIndicator);
	HealthIndicator->AddToViewport();
}

void AP10HUD::OnMissionCompleted(bool bSuccess)
{
	if (CrosshairWidget)
	{
		DrawCrosshair(false);
	}
	if (UUserWidget* MissionCompletedWidget = CreateWidget(GetWorld(), MissionCompletedWidgetClass))
	{
		MissionCompletedWidget->AddToViewport();
		// TODO: change text depends on bSuccess
	}
}
