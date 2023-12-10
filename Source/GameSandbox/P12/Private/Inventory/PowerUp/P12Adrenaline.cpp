// Copyright (C) 2023, IKinder

#include "P12/Public/Inventory/PowerUp/P12Adrenaline.h"

#include "P12/Public/Component/Actor/P12AttributeComponent.h"
#include "P12/Public/Player/P12BaseCharacter.h"

bool UP12Adrenaline::Consume(AP12BaseCharacter* ConsumeTarget)
{
	if (!ConsumeTarget)
	{
		return false;
	}
	UP12AttributeComponent* AttributeComponent = ConsumeTarget->GetAttributeComponent();
	check(AttributeComponent)
	AttributeComponent->RestoreFullStamina();
	ConditionalBeginDestroy();
	return true;
}
