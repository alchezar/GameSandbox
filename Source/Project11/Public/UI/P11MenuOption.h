// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11MenuOption.generated.h"

class USlider;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP11OnSliderValueChangedSignature, float, Value);

UCLASS()
class PROJECT11_API UP11MenuOption : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE USlider* GetSlider() const { return Slider; }

protected:
	virtual void NativePreConstruct() override;

	UFUNCTION()
	void OnSliderValueChangedHandle(float Value);

public:
	FP11OnSliderValueChangedSignature OnSliderValueChanged;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;
	UPROPERTY(meta = (BindWidget))
	USlider* Slider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FText CustomName = FText::FromString("setting");

private:
	float OldValue = 0.f;
};
