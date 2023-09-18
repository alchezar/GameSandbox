// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11StatPlayerScoreWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class GAMESANDBOX_API UP11StatPlayerScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPlayerName(const FText& NewName);
	void SetIsDead(const bool bNewDead);
	void SetKills(const int32 NewKills);
	void SetDeaths(const int32 NewDeaths);
	void SetPing(const float NewPing);

protected:
	virtual void NativePreConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillsText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeathText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PingText;
	UPROPERTY(meta = (BindWidget))
	UImage* DeadImg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FText PlayerName = FText::FromString("Name");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	int32 Kills = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	int32 Deaths = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	int32 Ping = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	bool bDead = false;
	
};
