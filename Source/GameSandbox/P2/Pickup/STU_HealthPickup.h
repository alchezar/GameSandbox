// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_BasePickup.h"
#include "STU_HealthPickup.generated.h"

UCLASS()
class GAMESANDBOX_API ASTU_HealthPickup : public ASTU_BasePickup
{
	GENERATED_BODY()

public:
	ASTU_HealthPickup();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	virtual bool TryToGivePickup(APawn* PlayerPawn) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"))
	float HealAmount = 50.f;
};
