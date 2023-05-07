// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ER_GameHud.generated.h"

class AER_GameModeBase;
class UTextBlock;

UCLASS()
class GAMESANDBOX_API UER_GameHud : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeHud(AER_GameModeBase* GameMode);
	void SetCoinsCount(int32 NewCount);
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CoinsCount;
};
