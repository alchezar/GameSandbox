// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "P11PlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class GAMESANDBOX_API AP11PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void MainMenuInput();

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputMappingContext* DefaultContext;
	UPROPERTY(EditAnywhere, Category = "C++ | Input")
	UInputAction* MainMenuAction;

private:
	bool bMenuVisibility = false;
};
