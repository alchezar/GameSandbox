// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TG_PatrolComponent.generated.h"

class ATargetPoint;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UTG_PatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTG_PatrolComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	TArray<ATargetPoint*> GetPatrolPoints() const;

protected:
	virtual void BeginPlay() override;

private:

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Kinder | AI")
	TArray<ATargetPoint*> PatrolPoints;
};
