// Copyright (C) 2023, IKinder

#include "ARWorldUserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void UARWorldUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DynamicHealthBar = HealthBarImage->GetDynamicMaterial();
}

void UARWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!AttachedActor)
	{
		RemoveFromParent();
		return;
	}
	
	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation(), ScreenPosition))
	{
		ScreenPosition /= UWidgetLayoutLibrary::GetViewportScale(this);
		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}

void UARWorldUserWidget::SetNewHealthPercent(const float HealthPercent)
{
	DynamicHealthBar->SetScalarParameterValue(ProgressName, HealthPercent);

	if (FMath::IsNearlyZero(HealthPercent))
		{
			RemoveFromParent();
		}
}