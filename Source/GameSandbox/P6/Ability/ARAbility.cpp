// Copyright (C) 2023, IKinder

#include "ARAbility.h"

UWorld* UARAbility::GetWorld() const
{
	// return UObject::GetWorld();
	
	/* Outer set in ARAbilityComponent by NewObject<>() */
	const UActorComponent* ActorComp = Cast<UActorComponent>(GetOuter());
	if (!ActorComp) return nullptr;

	return ActorComp->GetWorld();
}

void UARAbility::StartAction_Implementation(AActor* Instigator)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Running: %s"), *GetNameSafe(this)));
}

void UARAbility::StopAction_Implementation(AActor* Instigator)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Stopped: %s"), *GetNameSafe(this)));
}