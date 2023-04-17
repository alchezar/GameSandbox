// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "STU_Test_PickupCouldBeTaken.generated.h"

UCLASS()
class GAMESANDBOX_API USTU_Test_PickupCouldBeTaken : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	USTU_Test_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer);

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
