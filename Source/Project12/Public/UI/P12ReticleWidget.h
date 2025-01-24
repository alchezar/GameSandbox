// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P12ReticleWidget.generated.h"

class AP12BaseCharacter;

UCLASS()
class PROJECT12_API UP12ReticleWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	void OnAimStateChangedHandle(const bool bStart);

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ReticleAiming;

private:
	TWeakObjectPtr<AP12BaseCharacter> CachedCharacter;
};
