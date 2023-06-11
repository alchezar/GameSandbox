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

	SetHealthText(100.f);
	// BindHealthToAliveBody();
}

void UARCrosshairWidget::BindHealthToAliveBody(const AARCharacter* NewBody)
{
	if (!NewBody) return;
	NewBody->GetAttributesComp()->AROnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChangedHandle);
}

void UARCrosshairWidget::SetHealthText(const float NewHealth)
{
	TextBlockHealth->SetText(FText::AsNumber(NewHealth));
}

void UARCrosshairWidget::OnHealthChangedHandle(AActor* InstigatorActor, UARAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	SetHealthText(NewHealth);
}
