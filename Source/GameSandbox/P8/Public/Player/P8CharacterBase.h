// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P8CharacterBase.generated.h"

UCLASS()
class GAMESANDBOX_API AP8CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AP8CharacterBase();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
};
