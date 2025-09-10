// Copyright © 2025, Ivan Kinder

#include "Public/Character/AnimInstance/P17AnimInstanceBase.h"

#include "Project17.h"
#include "Util/P17FunctionLibrary.h"

bool UP17AnimInstanceBase::GetOwnerHaveTag(const FGameplayTag TagToCheck) const
{
	APawn* OwningPawn = TryGetPawnOwner();
	RETURN_IF(!OwningPawn, false)

	return UP17FunctionLibrary::NativeGetActorHasTag(OwningPawn, TagToCheck);
}
