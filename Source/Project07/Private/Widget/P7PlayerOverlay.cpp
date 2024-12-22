// Copyright (C) 2023, IKinder

#include "Widget/P7PlayerOverlay.h"

#include "Component/P7AttributeComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Player/P7Character.h"

bool UP7PlayerOverlay::Initialize()
{
	return Super::Initialize();
}

void UP7PlayerOverlay::NativeConstruct()
{
	Super::NativeConstruct();
}

void UP7PlayerOverlay::SetHealthPercent(const float Percent)
{
	HealthProgressBar->SetPercent(Percent);
}

void UP7PlayerOverlay::SetStaminaPercent(const float Percent)
{
	StaminaProgressBar->SetPercent(Percent);
}

void UP7PlayerOverlay::AddCoins(const int32 NewCoins)
{
	Coins += NewCoins;
	CoinCount->SetText(FText::AsNumber(Coins));
}

void UP7PlayerOverlay::AddSouls(const int32 NewSouls)
{
	Souls += NewSouls;
	SoulCount->SetText(FText::AsNumber(Souls));
}

void UP7PlayerOverlay::ConnectToCharacter(AP7Character* PlayerCharacter)
{
	Character = PlayerCharacter;
	Respawn();
	BindDelegates();
}

void UP7PlayerOverlay::Respawn()
{
	if (!Character) return;
	const UP7AttributeComponent* Attributes = Character->GetAttributes();
	if (!Attributes) return;
	SetHealthPercent(Attributes->GetHealthPercent());
	SetStaminaPercent(Attributes->GetStaminaPercent());
	AddCoins(Attributes->GetCoins());
	AddSouls(Attributes->GetSouls());
}

void UP7PlayerOverlay::BindDelegates()
{
	if (!Character) return;
	UP7AttributeComponent* Attributes = Character->GetAttributes();
	if (!Attributes) return;
	Attributes->OnReceiveDamage.AddUObject(this, &ThisClass::OnReceiveDamageHandle);
	Attributes->OnStaminaUsed.AddUObject(this, &ThisClass::OnStaminaUsedHandle);
	Attributes->OnReceiveGold.AddUObject(this, &ThisClass::OnReceiveGoldHandle);
	Attributes->OnReceiveSoul.AddUObject(this, &ThisClass::OnReceiveSoul);
}

void UP7PlayerOverlay::OnReceiveDamageHandle(ACharacter* Owner, float HealthPercent)
{
	if (Owner != Character) return;
	SetHealthPercent(HealthPercent);
}

void UP7PlayerOverlay::OnStaminaUsedHandle(ACharacter* Owner, float StaminaPercent)
{
	if (Owner != Character) return;
	SetStaminaPercent(StaminaPercent);
}

void UP7PlayerOverlay::OnReceiveGoldHandle(ACharacter* Owner, int32 NewGold)
{
	if (Owner != Character) return;
	AddCoins(NewGold);
}

void UP7PlayerOverlay::OnReceiveSoul(ACharacter* Owner, const int32 NewSouls)
{
	if (Owner != Character) return;
	AddSouls(NewSouls);
}
