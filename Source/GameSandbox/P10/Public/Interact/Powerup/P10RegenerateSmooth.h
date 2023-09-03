// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P10RegenerateHealth.h"
#include "P10RegenerateSmooth.generated.h"

UCLASS(HideCategories = "C++ | Steps")
class GAMESANDBOX_API AP10RegenerateSmooth : public AP10RegenerateHealth
{
	GENERATED_BODY()

public:
	AP10RegenerateSmooth();
	virtual void ActivatePowerup(AActor* Target) override;

protected:
	virtual void BeginPlay() override;
	
	virtual void OnActivated(AActor* Target) override;
	virtual void OnTickPowerup() override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Smooth")
	float Duration = 5.f;

};
