// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Project01/Public/Character/BatteryCollectorGameMode.h"
#include "PlayerHUD.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class PROJECT01_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UProgressBar* GetPowerBar() const;

	void UpdatePowerBar(const float Percent) const;
	void UpdateTextBlock(EPickupGameState CurrentState) const;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextBlockPower;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* PowerBar;

private:
};
