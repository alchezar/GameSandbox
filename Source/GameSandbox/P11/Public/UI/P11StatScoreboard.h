// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11StatScoreboard.generated.h"

class UP11StatPlayerScoreWidget;
class UVerticalBox;
class UTextBlock;

UCLASS()
class GAMESANDBOX_API UP11StatScoreboard : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void UpdateScore();

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayersCountText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MapNameText;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayersVBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UP11StatPlayerScoreWidget> PlayerScoreClass;

private:
	FTimerHandle UpdateTimer;
};
