// Copyright (C) 2023, IKinder

#include "P11/Public/UI/P11MenuOption.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"

void UP11MenuOption::NativePreConstruct()
{
	Super::NativePreConstruct();
	Slider->OnValueChanged.AddDynamic(this, &ThisClass::OnSliderValueChangedHandle);

	NameText->SetText(CustomName);
}

void UP11MenuOption::OnSliderValueChangedHandle(float Value)
{
	if (Value == OldValue)
	{
		return;
	}
	OldValue = Value;
	OnSliderValueChanged.Broadcast(Value);
}
