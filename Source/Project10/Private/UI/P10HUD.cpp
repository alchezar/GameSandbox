// Copyright (C) 2023, IKinder

#include "UI/P10HUD.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "UI/P10MissionCompleteWidget.h"

AP10HUD::AP10HUD(): CrosshairTex(nullptr), CrosshairWidget(nullptr), HealthIndicator(nullptr), MissionCompletedWidget(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AP10HUD::BeginPlay()
{
	Super::BeginPlay();
	check(CrosshairWidgetClass)
	check(MissionCompletedWidgetClass)
	check(HealthIndicatorWidgetClass)

	PlayerSpawned();
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

void AP10HUD::DestroyHealthIndicator() const
{
	if (!HealthIndicator) return;
	HealthIndicator->RemoveFromParent();
}

void AP10HUD::OnMissionCompleted(bool bSuccess, const float ShowTime)
{
	if (CrosshairWidget)
	{
		DrawCrosshair(false);
	}
	MissionCompletedWidget = CreateWidget<UP10MissionCompleteWidget>(GetWorld(), MissionCompletedWidgetClass);
	if (!MissionCompletedWidget) return;

	MissionCompletedWidget->SetMissionText(bSuccess ? "mission completed" : "mission failed");
	MissionCompletedWidget->AddToViewport();

	FTimerHandle ShowTimer;
	GetWorld()->GetTimerManager().SetTimer(ShowTimer, this, &ThisClass::DestroyMissionComplete, ShowTime, false);
}

void AP10HUD::DestroyMissionComplete() const
{
	if (!MissionCompletedWidget) return;
	MissionCompletedWidget->RemoveFromParent();
}

void AP10HUD::PlayerDied()
{
	DrawCrosshair(false);
	DestroyHealthIndicator();
}

void AP10HUD::PlayerSpawned()
{
	DrawCrosshair(true);
	DrawHealthIndicator();
}
