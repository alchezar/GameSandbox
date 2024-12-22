// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STU_PlayerStatRowWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class GAMESANDBOX_API USTU_PlayerStatRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPlayerName(const FText& Text);
	void SetKills(const FText& Text);
	void SetDeaths(const FText& Text);
	void SetTeams(const FText& Text);
	void SetPlayerIndicatorVisibility(bool Visible);
	void SetTeamColor(const FLinearColor& Color);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillsText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeathsText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TeamText;
	UPROPERTY(meta = (BindWidget))
	UImage* PlayerIndicatorImage;
	UPROPERTY(meta = (BindWidget))
	UImage* TeamImage;
};
