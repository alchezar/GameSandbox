// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P4/Player/TG_BaseCharacter.h"
#include "TG_AI_Character.generated.h"

class ATargetPoint;

UCLASS()
class GAMESANDBOX_API ATG_AI_Character : public ATG_BaseCharacter
{
	GENERATED_BODY()

public:
	ATG_AI_Character();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void CharacterDying() override;
};
