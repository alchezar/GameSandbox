// Copyright (C) 2023, IKinder

#include "ARCrosshairWidget.h"
#include "Components/TextBlock.h"
#include "P6/Component/ARAttributesComponent.h"
#include "P6/Player/ARCharacter.h"

bool UARCrosshairWidget::Initialize()
{
	return Super::Initialize();
}

void UARCrosshairWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindDelegates();
	SetHealthText(100.f);
}

void UARCrosshairWidget::BindDelegates()
{
	const AARCharacter* Player = Cast<AARCharacter>(GetOwningPlayerPawn());
	if (!Player) return;

	Player->GetAttributesComp()->AROnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChangedHandle);
}

void UARCrosshairWidget::SetHealthText(const float NewHealth)
{
	TextBlockHealth->SetText(FText::AsNumber(NewHealth));
}

void UARCrosshairWidget::OnHealthChangedHandle(AActor* InstigatorActor, UARAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	SetHealthText(NewHealth);
}
