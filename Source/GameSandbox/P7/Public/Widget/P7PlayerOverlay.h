// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P7PlayerOverlay.generated.h"

class AP7Character;
class UTextBlock;
class UProgressBar;

UCLASS()
class GAMESANDBOX_API UP7PlayerOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	void SetHealthPercent(const float Percent);
	void SetStaminaPercent(const float Percent);
	void AddCoins(const int32 NewCoins);
	void AddSouls(const int32 NewSouls);
	void SetPlayerCharacter(AP7Character* PlayerCharacter);

protected:
	virtual void NativeConstruct() override;

private:
	void Respawn();
	void BindDelegates();
	void OnReceiveDamageHandle(float HealthPercent);
	void OnReceiveGoldHandle(int32 NewGold);
	void OnReceiveSoul(const int32 NewSouls);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgressBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoinCount;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoulCount;

private:
	int32 Coins = 0;
	int32 Souls = 0;
	UPROPERTY()
	AP7Character* Character;
};
