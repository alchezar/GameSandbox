// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17UIPawnComponent.h"
#include "P17UIEnemyComponent.generated.h"

class UP17WidgetBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT17_API UP17UIEnemyComponent : public UP17UIPawnComponent
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	UP17UIEnemyComponent();

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyDrawWidget(UP17WidgetBase* InWidgetToRegister);
	UFUNCTION(BlueprintCallable)
	void RemoveEnemyDrawWidgets();

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
private:
	UPROPERTY()
	TArray<UP17WidgetBase*> EnemyDrawnWidgets = {};
};
