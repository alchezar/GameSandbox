// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TG_AI_Character.generated.h"

class ATargetPoint;

UCLASS()
class GAMESANDBOX_API ATG_AI_Character : public ACharacter
{
	GENERATED_BODY()

public:
	ATG_AI_Character();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
};
