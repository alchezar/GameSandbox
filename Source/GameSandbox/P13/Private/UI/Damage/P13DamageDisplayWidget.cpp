// Copyright © 2024, IKinder

#include "P13/Public/UI/Damage/P13DamageDisplayWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextBlock.h"
#include "Curves/CurveLinearColor.h"

void UP13DamageDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(LiftAnim);
	
	FTimerHandle RemoveTimer;
	FTimerDelegate RemoveDelegate;
	RemoveDelegate.BindLambda([&](){ RemoveFromParent(); });
	GetWorld()->GetTimerManager().SetTimer(RemoveTimer, RemoveDelegate, ShowTime, false);
}

void UP13DamageDisplayWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateScreenLocation();
}

void UP13DamageDisplayWidget::InitWidget(const USceneComponent* ComponentToAttach, const float CurrentDamage, const float NewHealthAlpha, const FVector2D NewRandomOffset)
{
	CachedComponentToAttach = ComponentToAttach;
	DamageText->SetText(FText::AsNumber(CurrentDamage));
	HealthAlpha = NewHealthAlpha;
	RandomOffset = NewRandomOffset;
}

void UP13DamageDisplayWidget::UpdateScreenLocation()
{
	if (!CachedComponentToAttach.IsValid())
	{
		return;
	}
	const APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController)
	{
		return;
	}

	FVector2D ScreenPosition;
	const bool bInViewport = PlayerController->ProjectWorldLocationToScreen(CachedComponentToAttach->GetComponentLocation(), ScreenPosition);
	const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	const FVector2D AnchorPoint = ScreenPosition / ViewportScale;
	const FLinearColor DamageColor = ColorOverLife ? ColorOverLife->GetLinearColorValue(HealthAlpha) : FColor::White;

	DamageText->SetRenderTranslation(AnchorPoint + RandomOffset);
	DamageText->SetVisibility(bInViewport ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	DamageText->SetColorAndOpacity(DamageColor);
}
