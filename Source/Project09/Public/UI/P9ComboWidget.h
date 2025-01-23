// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/UserWidget.h"
#include "P9ComboWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class PROJECT09_API UP9ComboWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void UpdateCompoCount(const int32 Value);
	void ResetCombo() const;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextCombo;

private:
	TMap<FName, UWidgetAnimation> AnimMap;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* AnimComboFade;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* AnimComboShake;
};
