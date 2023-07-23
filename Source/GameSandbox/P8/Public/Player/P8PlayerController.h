// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "P8/Public/Interface/P8PlayerControllerInterface.h"
#include "P8PlayerController.generated.h"

class UP8InGameMenuWidget;
class UInputAction;
class UInputMappingContext;

UCLASS()
class GAMESANDBOX_API AP8PlayerController : public APlayerController, public IP8PlayerControllerInterface
{
	GENERATED_BODY()

public:
	AP8PlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void UnPause() override;   // IP8PlayerControllerInterface
	virtual void GoToLobby() override; // IP8PlayerControllerInterface
	void SwitchGamePause();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputMappingContext* DefaultContext;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	UInputAction* GamePauseAction;

private:
	bool bGamePause = false;
	UPROPERTY()
	UP8InGameMenuWidget* GamePauseWidget;
};
