// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16CharacterBase.h"
#include "Interface/P16InterfaceEnemy.h"
#include "Util/P16Util.h"
#include "P16Enemy.generated.h"

class UWidgetComponent;

UCLASS()
class PROJECT16_API AP16Enemy : public AP16CharacterBase, public IP16InterfaceEnemy
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16Enemy();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaTime) override;

	/// ------------------------------------------------------------------------
	/// @name Interface
	/// ------------------------------------------------------------------------
public:
	virtual void  ToggleHighlight(const bool bOn) override;
	virtual int32 GetPlayerLevel() override { return Level; };

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
protected:
	virtual void InitAbilityActorInfo() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
private:
	void InitHealthBar();

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(BlueprintAssignable)
	FP16OnAttributeChangeSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FP16OnAttributeChangeSignature OnMaxHealthChanged;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UWidgetComponent> HealthBar = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	int32 Level = 1;
};
