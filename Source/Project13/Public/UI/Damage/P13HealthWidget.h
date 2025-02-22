// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P13HealthWidget.generated.h"

class UProgressBar;

UCLASS()
class PROJECT13_API UP13HealthWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
protected:
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
protected:
	void OnShieldChangedHandle(const float NewShield, const float LastDamage, const float ShieldAlpha) const;
	void OnHealthChangedHandle(const float NewHealth, const float LastDamage, const float HealthAlpha) const;
	void OnDeathHandle(AController* Causer);

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ShieldBar;
};
