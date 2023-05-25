// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TG_WidgetMain.generated.h"

class ATG_FirstPersonCharacter;
class ATG_GameMode;
class UTextBlock;

UCLASS()
class GAMESANDBOX_API UTG_WidgetMain : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

	void BindDelegates(ATG_GameMode* TheGameMode, ATG_FirstPersonCharacter* ThePlayer);
	void SetHealth(const float NewHealth);
	void SetScore(const int32 NewScore);

private:
	void OnHeathChangedHandle(float NewHealth);
	void OnScoreChangeHandle(int32 NewScore);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;
};
