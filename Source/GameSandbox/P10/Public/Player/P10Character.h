// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P10Character.generated.h"

UCLASS()
class GAMESANDBOX_API AP10Character : public ACharacter
{
	GENERATED_BODY()

public:
	AP10Character();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
};
