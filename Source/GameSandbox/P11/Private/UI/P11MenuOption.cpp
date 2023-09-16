// Copyright (C) 2023, IKinder

#include "P11/Public/UI/P11MenuOption.h"

#include "Components/TextBlock.h"

void UP11MenuOption::NativePreConstruct()
{
	Super::NativePreConstruct();

	NameText->SetText(CustomName);
}
