// Copyright (C) 2023, IKinder

#include "PlayerAI/EQS/STU_Test_PickupCouldBeTaken.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Pickup/STU_BasePickup.h"

USTU_Test_PickupCouldBeTaken::USTU_Test_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Cost          = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_Actor::StaticClass();
	SetWorkOnFloatValues(false);
}

void USTU_Test_PickupCouldBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{
	const UObject* DataOwner = QueryInstance.Owner.Get();
	BoolValue.BindData(DataOwner, QueryInstance.QueryID);
	const bool bWantsBeTakable = BoolValue.GetValue();

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		AActor*                ItemActor = GetItemActor(QueryInstance, It.GetIndex());
		const ASTU_BasePickup* Pickup    = Cast<ASTU_BasePickup>(ItemActor);
		if (!Pickup) continue;

		const bool CouldBeTaken = Pickup->CouldBeTaken();
		It.SetScore(TestPurpose, FilterType, CouldBeTaken, bWantsBeTakable);
	}
}
