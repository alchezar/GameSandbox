// Copyright (C) 2023, IKinder

#include "P11/Public/UI/P11MenuPopUp.h"

#include "Components/TextBlock.h"

void UP11MenuPopUp::NativePreConstruct()
{
	Super::NativePreConstruct();
	NameText->SetText(CustomName);
}
