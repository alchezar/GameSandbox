// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Component/P10HealthComponent.h"
#include "P10HealthIndicator.generated.h"

class UImage;

UCLASS()
class PROJECT10_API UP10HealthIndicator : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	void OnHealthChangedHandle(UP10HealthComponent* HealthComponent, float Health, float Delta, const UDamageType* DamageType, AController* InstignatedBy, AActor* DamageCauser);

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Logo;

private:
	UPROPERTY()
	UP10HealthComponent* HealthComp;
};
