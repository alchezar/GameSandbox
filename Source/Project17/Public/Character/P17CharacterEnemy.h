// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17CharacterBase.h"
#include "P17CharacterEnemy.generated.h"

#if 0
class UBoxComponent;
#endif
class UWidgetComponent;
class UP17UIEnemyComponent;
class UP17CombatEnemyComponent;
struct FStreamableHandle;

UCLASS()
class PROJECT17_API AP17CharacterEnemy : public AP17CharacterBase
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP17CharacterEnemy();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name Interface
	/// ------------------------------------------------------------------------
public:
	/// @par IP17CombatInterface -----------------------------------------------
	virtual UP17CombatPawnComponent* GetCombatComponent() const override;

	/// @par IP17UIInterface -----------------------------------------------
	virtual UP17UIPawnComponent* GetPawnUIComponent() const override;
	virtual UP17UIEnemyComponent* GetEnemyUIComponent() const override;

#if 0
protected:
	UFUNCTION()
	void OnHandOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
#endif

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable, Category = "C++ | Component")
	UP17CombatEnemyComponent* GetEnemyCombatComponent() const { return CombatComponent; };
	UFUNCTION(BlueprintCallable, Category = "C++ | Component")
	UP17UIEnemyComponent* GetUIComponent() const { return UIComponent; };
#if 0
	UFUNCTION(BlueprintCallable, Category = "C++ | Component")
	UBoxComponent* GetLeftHandCollision() const { return LeftHandCollision; }
	UFUNCTION(BlueprintCallable, Category = "C++ | Component")
	UBoxComponent* GetRightHandCollision() const { return RightHandCollision; }
#endif

private:
	void InitEnemyStartupData() const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UP17CombatEnemyComponent> CombatComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UP17UIEnemyComponent> UIComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UWidgetComponent> HealthWidgetComponent = nullptr;
#if 0
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UBoxComponent> LeftHandCollision = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UBoxComponent> RightHandCollision = nullptr;
#endif
};
