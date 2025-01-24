// Copyright (C) 2023, IKinder

#include "Inventory/PowerUp/P12Medkit.h"

#include "Component/Actor/P12AttributeComponent.h"
#include "Player/P12BaseCharacter.h"

bool UP12Medkit::Consume(AP12BaseCharacter* ConsumeTarget)
{
	if (!ConsumeTarget)
	{
		return false;
	}
	UP12AttributeComponent* AttributeComponent = ConsumeTarget->GetAttributeComponent();
	check(AttributeComponent)
	AttributeComponent->AddHealth(Health);
	ConditionalBeginDestroy();
	return true;
}
