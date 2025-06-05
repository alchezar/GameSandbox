// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16SummonAbility.h"

TArray<FVector> UP16SummonAbility::GetSpawnLocations() const
{
	const FVector Location = GetOwningActorFromActorInfo()->GetActorLocation();
	const FVector Forward  = GetOwningActorFromActorInfo()->GetActorForwardVector();

	const FVector SpreadLeftBound = Forward.RotateAngleAxis(SpawnSpreadAngle / -2.f, FVector::UpVector);
	const float   SpreadDelta     = SpawnSpreadAngle / FMath::Max(1.f, MinionsNum - 1.f);

	TArray<FVector> Result = {};
	Result.Reserve(MinionsNum);
	for (int32 Index = 0; Index < MinionsNum; ++Index)
	{
		const FVector Direction     = SpreadLeftBound.RotateAngleAxis(SpreadDelta * Index, FVector::UpVector);
		const float   SpawnRange    = FMath::RandRange(SpawnDistanceRange.X, SpawnDistanceRange.Y);
		FVector       SpawnLocation = Location + Direction * SpawnRange;

		FHitResult    Hit;
		const FVector Start = SpawnLocation + FVector::UpVector * 400.f;
		const FVector End   = SpawnLocation - FVector::UpVector * 400.f;
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility))
		{
			SpawnLocation = Hit.Location;
		}

		Result.Add(SpawnLocation);
	}

	return Result;
}

TSubclassOf<APawn> UP16SummonAbility::GetRandomMinionClass() const
{
	const int32 Selection = FMath::RandRange(0, MinionsClasses.Num() - 1);
	return MinionsClasses[Selection];
}
