// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "P13PlayerStatsWidget.generated.h"

class AP13PlayerState;
class UTextBlock;

UCLASS()
class GAMESANDBOX_API UP13PlayerStatsWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

	/* ------------------------------- This -------------------------------- */
private:
	void OnPlayerScoreChangedHandle(const int32 NewScore);
	void OnPlayerLivesChangedHandle(const int32 NewLives);
	void OnOneSecondPassed();

private:
	static FString NumberToTime(const int32 Time);

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LivesText;

private:
	int32 SecondsFromStart = 0;
	TWeakObjectPtr<AP13PlayerState> PlayerStateCached;
};