// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "P10PlayerState.generated.h"

UCLASS()
class PROJECT10_API AP10PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void AddScore(const float ScoreDelta);
	void ResetScore();
};
