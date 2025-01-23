// Copyright (C) 2023, IKinder

#include "UI/P11MenuPopUp.h"

#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"

void UP11MenuPopUp::NativePreConstruct()
{
	Super::NativePreConstruct();
	ComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::OnComboBoxSelectionChangedHandle);
	
	NameText->SetText(CustomName);
	
}

void UP11MenuPopUp::OnComboBoxSelectionChangedHandle(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	OnComboBoxSelectionChanged.Broadcast(SelectedItem);
}
