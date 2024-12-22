// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STU_RespawnComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API USTU_RespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USTU_RespawnComponent();
	void Respawn(int32 RespawnTime);
	int32 GetRespawnCountDown() const;
	bool IsRespawnInProgress() const;

protected:
	virtual void BeginPlay() override;

private:
	void RespawnTimerUpdate();

	FTimerHandle RespawnTimer;
	int32        RespawnCountDown = 0;
};
