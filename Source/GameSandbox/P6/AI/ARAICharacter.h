// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARAICharacter.generated.h"

UCLASS()
class GAMESANDBOX_API AARAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AARAICharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:
};
