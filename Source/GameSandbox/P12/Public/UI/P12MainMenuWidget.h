// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P12/Public/Player/P12FirstPersonCharacter.h"
#include "P12MainMenuWidget.generated.h"

class UButton;

UCLASS()
class GAMESANDBOX_API UP12MainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void CacheHUD(const AP12HUD* HUD);
	
protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnReturnButtonClickedHandle();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ReturnButton;

private:
	TSoftObjectPtr<AP12HUD> CachedHUD;
};
