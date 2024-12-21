// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STU_Menu_PlayerController.generated.h"

UCLASS()
class PROJECT02_API ASTU_Menu_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
