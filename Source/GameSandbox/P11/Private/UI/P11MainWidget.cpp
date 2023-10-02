// Copyright (C) 2023, IKinder

#include "P11/Public/UI/P11MainWidget.h"

#include "Components/TextBlock.h"
#include "P11/Public/Player/P11Character.h"

void UP11MainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController)
	{
		return;
	}
	APawn* ControlledPawn = PlayerController->GetPawn();
	if (!ControlledPawn)
	{
		return;
	}
	CurrentCharacter = Cast<AP11Character>(ControlledPawn);
	if (!CurrentCharacter)
	{
		return;
	}
	CurrentCharacter->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChangedHandle);
	CurrentCharacter->OnAmmoChanged.AddDynamic(this, &ThisClass::OnAmmoChangedHandle);
	OnHealthChangedHandle(CurrentCharacter->GetHealth());
	OnAmmoChangedHandle(CurrentCharacter->GetAmmo());
}

void UP11MainWidget::OnHealthChangedHandle(const float Health)
{
	HealthText->SetText(FText::AsNumber(Health));
}

void UP11MainWidget::OnAmmoChangedHandle(const int32 Ammo)
{
	AmmoText->SetText(FText::AsNumber(Ammo));
}
