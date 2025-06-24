// Copyright © 2025, Ivan Kinder

#include "Player/AnimInstance/P17LinkedAnimLayer.h"

#include "Player/AnimInstance/P17AnimInstanceHero.h"

UP17AnimInstanceHero* UP17LinkedAnimLayer::GetHeroAnimInstance() const
{
	return Cast<UP17AnimInstanceHero>(GetOwningComponent()->GetAnimInstance());
}
