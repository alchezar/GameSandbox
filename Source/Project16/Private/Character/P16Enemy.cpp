// Copyright © 2025, Ivan Kinder

#include "Character/P16Enemy.h"

#include "Project16.h"

AP16Enemy::AP16Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// For enemies highlight on hover.
	GetMesh()->SetCustomDepthStencilValue(P16::CustomDepthRed);
	Weapon->SetCustomDepthStencilValue(P16::CustomDepthRed);
}

void AP16Enemy::BeginPlay()
{
	Super::BeginPlay();
}

void AP16Enemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP16Enemy::ToggleHighlight(const bool bOn)
{
	GetMesh()->SetRenderCustomDepth(bOn);
	Weapon->SetRenderCustomDepth(bOn);
}
