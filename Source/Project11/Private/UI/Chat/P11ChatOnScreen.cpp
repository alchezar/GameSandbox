// Copyright (C) 2023, IKinder

#include "UI/Chat/P11ChatOnScreen.h"

#include "UI/Chat/P11ChatBox.h"

void UP11ChatOnScreen::NativeConstruct()
{
	Super::NativeConstruct();
}

void UP11ChatOnScreen::ExtendChat(const bool bExtended) const
{
	ChatBox->ExtendChat(bExtended);
}
