// Copyright (C) 2023, IKinder

#include "UI/ARCrosshairWidget.h"

#include "Component/ARAttributesComponent.h"
#include "Components/TextBlock.h"
#include "Player/ARCharacter.h"
#include "Player/ARPlayerController.h"

bool UARCrosshairWidget::Initialize()
{
	return Super::Initialize();
}

void UARCrosshairWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindDelegates();
}

void UARCrosshairWidget::BindDelegates()
{
	AARPlayerController* Controller = Cast<AARPlayerController>(GetOwningPlayer());
	if (!Controller) return;

	Controller->OnPawnChanged.AddDynamic(this, &ThisClass::OnPawnChangedHandle);
	OnPawnChangedHandle(Controller->GetPawn());
}

void UARCrosshairWidget::OnPawnChangedHandle(APawn* NewPawn)
{
	const AARCharacter* Player = Cast<AARCharacter>(NewPawn);
	if (!Player) return;
	UARAttributesComponent* AttributesComp = Player->FindComponentByClass<UARAttributesComponent>();
	if (!AttributesComp) return;

	AttributesComp->AROnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChangedHandle);
	OnHealthChangedHandle(nullptr, AttributesComp, AttributesComp->GetHealth(), 0.f);
}

void UARCrosshairWidget::BindHealthToAliveBody(const AARCharacter* NewBody)
{
	if (!NewBody) return;
	NewBody->GetAttributesComp()->AROnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChangedHandle);
}

void UARCrosshairWidget::OnHealthChangedHandle(AActor* InstigatorActor, UARAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	TextBlockHealth->SetText(FText::AsNumber(NewHealth));
}