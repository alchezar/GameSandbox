// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARPauseMenu.generated.h"

class UARMenuButton;

UCLASS()
class GAMESANDBOX_API UARPauseMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnReturnToMainClicked();
	UFUNCTION()
	void OnReturnToGameClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	UARMenuButton* ToMainButton;
	UPROPERTY(meta = (BindWidget))
	UARMenuButton* ToGameButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FName MainLevelName = "AR_Entry";
};
