// Copyright (C) 2023, IKinder

#include "P10/Public/UI/P10HealthIndicator.h"

#include "Components/Image.h"
#include "P10/Public/Component/P10HealthComponent.h"

void UP10HealthIndicator::NativeConstruct()
{
	Super::NativeConstruct();

	if (!GetOwningPlayerPawn()) return;
	HealthComp = GetOwningPlayerPawn()->GetComponentByClass<UP10HealthComponent>();
	if (!HealthComp) return;
	HealthComp->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChangedHandle);
}

void UP10HealthIndicator::OnHealthChangedHandle(UP10HealthComponent* HealthComponent, float Health, float Delta, const UDamageType* DamageType, AController* InstignatedBy, AActor* DamageCauser)
{
	UMaterialInstanceDynamic* LogoMaterial = Logo->GetDynamicMaterial();
	if (!LogoMaterial) return;
	
	const float HealthAlpha = FMath::Clamp(Health / HealthComponent->GetMaxHealth(), 0.f, 1.f);
	LogoMaterial->SetScalarParameterValue(FName("Alpha"), HealthAlpha);
}
