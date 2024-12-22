// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ER_GameOver.generated.h"

class UTextBlock;
class AER_GameModeBase;
class UButton;

UCLASS()
class GAMESANDBOX_API UER_GameOver : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowFinalScore(const int32 Score);

protected:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void OnRestartClicked();
	UFUNCTION()
	void OnMainClicked();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* RestartBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* MainBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ScoreText;

private:
	UPROPERTY()
	AER_GameModeBase* GameMode;
};


