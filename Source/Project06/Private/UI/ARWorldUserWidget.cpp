// Copyright (C) 2023, IKinder

#include "UI/ARWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Component/ARAbilityComponent.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "UI/AREffectContainer.h"

void UARWorldUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (AttachedActor)
	{
		UARAbilityComponent* AbilityComp = AttachedActor->FindComponentByClass<UARAbilityComponent>();
		if (!AbilityComp) return;

		EffectContainer->SetAbilityComponent(AbilityComp);
	}
	
	DynamicHealthBar = HealthBarImage->GetDynamicMaterial();

	FTimerHandle RemoveTimer;
	GetWorld()->GetTimerManager().SetTimer(RemoveTimer, [&](){ RemoveFromParent(); }, 1.f, false);

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
	if (const bool bInViewport = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation(), ScreenPosition))
	{
		ScreenPosition /= UWidgetLayoutLibrary::GetViewportScale(this);
		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
			ParentSizeBox->SetVisibility(bInViewport ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
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
