// Copyright (C) 2023, IKinder

#include "P7/Public/Widget/P7PlayerOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "P7/Public/Component/P7AttributeComponent.h"
#include "P7/Public/Player/P7Character.h"

bool UP7PlayerOverlay::Initialize()
{
	return Super::Initialize();
}

void UP7PlayerOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	Respawn();
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

void UP7PlayerOverlay::SetPlayerCharacter(AP7Character* PlayerCharacter)
{
	if (!PlayerCharacter) return;
	Character = PlayerCharacter;
	BindDelegates();
}

void UP7PlayerOverlay::Respawn()
{
	SetHealthPercent(1.f);
	SetStaminaPercent(1.f);
	AddCoins(0);
	AddSouls(0);
}

void UP7PlayerOverlay::BindDelegates()
{
	if (!Character) return;
	UP7AttributeComponent* Attributes = Character->GetAttributes();
	if (!Attributes) return;
	Attributes->OnReceiveDamage.AddUObject(this, &ThisClass::OnReceiveDamageHandle);
	Attributes->OnReceiveGold.AddUObject(this, &ThisClass::OnReceiveGoldHandle);
	Attributes->OnReceiveSoul.AddUObject(this, &ThisClass::OnReceiveSoul);
}

void UP7PlayerOverlay::OnReceiveDamageHandle(float HealthPercent)
{
	SetHealthPercent(HealthPercent);
}

void UP7PlayerOverlay::OnReceiveGoldHandle(int32 NewGold)
{
	AddCoins(NewGold);
}

void UP7PlayerOverlay::OnReceiveSoul(const int32 NewSouls)
{
	AddSouls(NewSouls);
}
