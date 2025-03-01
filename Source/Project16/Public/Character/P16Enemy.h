// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16CharacterBase.h"
#include "Interface/P16InterfaceEnemy.h"
#include "P16Enemy.generated.h"

UCLASS()
class PROJECT16_API AP16Enemy : public AP16CharacterBase, public IP16InterfaceEnemy
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP16Enemy();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaTime) override;

	/* ----------------------------- Interface ----------------------------- */
public:
	virtual void ToggleHighlight(const bool bOn) override;

	/* ------------------------------- Super ------------------------------- */
protected:
	virtual void InitAbilityActorInfo() override;
};
