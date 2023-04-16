// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "STU_AIPerceptionComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API USTU_AIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	USTU_AIPerceptionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AActor* GetClosestEnemy() const;

protected:
	virtual void BeginPlay() override;
};
