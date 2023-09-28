// Copyright (C) 2023, IKinder

#include "P11/Public/UI/ServerList/P11ServerCreate.h"

// #include "Components/Button.h"

void UP11ServerCreate::NativeConstruct()
{
	Super::NativeConstruct();

	// BackBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackHandle);
	// CreateBtn->OnClicked.AddDynamic(this, &ThisClass::OnCreateHandle);
}

void UP11ServerCreate::OnBackHandle()
{
	// Super::OnBackHandle();
	OnCreateBackButtonPressed.Broadcast();
}

void UP11ServerCreate::OnCreateHandle()
{
	Super::OnCreateHandle();
}
