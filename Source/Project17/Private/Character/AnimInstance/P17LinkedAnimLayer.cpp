// Copyright © 2025, Ivan Kinder

#include "Public/Character/AnimInstance/P17LinkedAnimLayer.h"

#include "Public/Character/AnimInstance/P17AnimInstanceHero.h"

UP17AnimInstanceHero* UP17LinkedAnimLayer::GetHeroAnimInstance() const
{
	return Cast<UP17AnimInstanceHero>(GetOwningComponent()->GetAnimInstance());
}
