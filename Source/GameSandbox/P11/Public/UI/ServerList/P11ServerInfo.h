// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11ServerInfo.generated.h"

class UTextBlock;

UCLASS()
class GAMESANDBOX_API UP11ServerInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerNameText; 
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayersText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PingText;
};
