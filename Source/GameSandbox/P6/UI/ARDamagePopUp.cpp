// Copyright (C) 2023, IKinder

#include "ARDamagePopUp.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

bool UARDamagePopUp::Initialize()
{
	return Super::Initialize();
}

void UARDamagePopUp::NativeConstruct()
{
	Super::NativeConstruct();

	FTimerHandle RemoveTimer;
	GetWorld()->GetTimerManager().SetTimer(RemoveTimer, [&]()
	{
		RemoveFromParent();
	}, 1.f, false);
}

void UARDamagePopUp::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateScreenLocation();
}

void UARDamagePopUp::UpdateScreenLocation() const
{
	if (!ActorToAttach) return;

	FVector2D ScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), ActorToAttach->GetActorLocation(), ScreenPosition);
	DamageText->SetRenderTranslation(ScreenPosition / UWidgetLayoutLibrary::GetViewportScale(GetWorld()));
}

void UARDamagePopUp::SetDamageText(const float Delta)
{
	DamageText->SetText(FText::AsNumber(Delta));
}

void UARDamagePopUp::SetActorToAttach(AActor* ToAttach)
{
	ActorToAttach = ToAttach;
}
