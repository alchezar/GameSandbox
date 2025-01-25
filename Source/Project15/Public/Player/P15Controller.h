// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "P15Controller.generated.h"

UCLASS()
class PROJECT15_API AP15Controller : public APlayerController
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP15Controller();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/* ------------------------------- This -------------------------------- */

	/* ------------------------------ Fields ------------------------------- */
};
