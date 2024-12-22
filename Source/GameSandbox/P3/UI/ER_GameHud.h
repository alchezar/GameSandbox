// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ER_GameHud.generated.h"

class UButton;
class AER_GameModeBase;
class UTextBlock;

UCLASS()
class GAMESANDBOX_API UER_GameHud : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeHud(AER_GameModeBase* GameMode);
	void SetCoinsCount(int32 NewCount);
	void SetLivesCount(int32 NewCount);

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnPauseClicked();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Pause")
	TSubclassOf<UUserWidget> PauseWidgetClass;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CoinsCount;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* LivesCount;
	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
	UButton* PauseBtn;
};
