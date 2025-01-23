// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11MenuFPS.generated.h"

class UTextBlock;

UCLASS()
class PROJECT11_API UP11MenuFPS : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void ShowFpsEverySecond() const;
	UFUNCTION()
	void OnVisibilityChangedHandle(ESlateVisibility InVisibility);
	
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* FpsText;

private:
	FTimerHandle FpsTimer;
};
