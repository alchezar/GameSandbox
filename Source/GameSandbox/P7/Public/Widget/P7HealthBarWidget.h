// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P7HealthBarWidget.generated.h"

class UProgressBar;

UCLASS()
class GAMESANDBOX_API UP7HealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	void SetHealthPercent(const float Percent) const;

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
};
