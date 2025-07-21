// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Abilities/Traceable/P17TraceableAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FHitResult> UP17TraceableAbility::BoxTrace() const
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	const FVector Forward = Avatar->GetActorForwardVector();
	const FVector Start = Avatar->GetActorLocation();
	const FVector End = Start + Forward * TraceDistance;

	// clang-format off
	const EDrawDebugTrace::Type DebugType = bShowDebug
		? bPersistent
			? EDrawDebugTrace::Persistent
			: EDrawDebugTrace::ForOneFrame
		: EDrawDebugTrace::None;
	// clang-format on

	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		Avatar,
		Start,
		End,
		TraceBoxSize / 2,
		Forward.Rotation(),
		TraceChannels,
		false,
		{Avatar},
		DebugType,
		HitResults,
		true);

	return HitResults;
}
