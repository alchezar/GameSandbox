// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "P11ServerInfo.generated.h"

class UP11GameInstance;
class UTextBlock;

UCLASS()
class PROJECT11_API UP11ServerInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSessionInfo(const FOnlineSessionSearchResult& SessionSearchResult);
	
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerNameText; 
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayersText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PingText;
	
private:
	FOnlineSessionSearchResult Session;
	UPROPERTY()
	UP11GameInstance* GameInstance;
};
