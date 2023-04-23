// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_BaseWidget.h"
#include "STU_CoreTypes.h"
#include "STU_GameOverWidget.generated.h"

class ASTU_PlayerState;
class UVerticalBox;
class UButton;

UCLASS()
class GAMESANDBOX_API USTU_GameOverWidget : public USTU_BaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayerStatBox;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* EnemyStatBox;
	UPROPERTY(meta = (BindWidget))
	UButton* ResetLevelButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | UI")
	TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

private:
	void OnMatchStateChanged(ESTU_MatchState State);
	void UpdatePlayerStat();
	UFUNCTION()
	void OnResetLevel();

	static FText TextFromInt(int32 Num);
};
