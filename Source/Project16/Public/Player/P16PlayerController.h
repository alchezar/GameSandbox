// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Util/P16Type.h"
#include "P16PlayerController.generated.h"

class AP16Character;
class AP16MagicCircle;
class IP16HighlightInterface;
class UInputAction;
class UInputMappingContext;
class UNiagaraSystem;
class UP16AbilitySystemComponent;
class UP16DamageTextComponent;
class UP16InputConfig;
class USplineComponent;
struct FGameplayTag;
struct FInputActionValue;

UCLASS()
class PROJECT16_API AP16PlayerController : public APlayerController
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16PlayerController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaSeconds) override;
	virtual void SetupInputComponent() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	_NODISCARD
	FHitResult GetCursorHit() const { return CursorHit; };

	UFUNCTION(Client, Unreliable)
	void Client_ShowDamageNumber(const float InDamage, AActor* Target, const bool bBlockedHit, const bool bCriticalHit);
	UFUNCTION(BlueprintCallable)
	void ToggleMagicCircle(const bool bOn, UMaterialInterface* DecalMaterial = nullptr);

protected:
	void MoveInputCallback(const FInputActionValue& InputValue);
	void ShiftInputCallback(const FInputActionValue& InputValue, const bool bDown);
	void CursorTrace();
	void AutoRun();
	void UpdateMagicCircle() const;

	void AbilityInputTagPressed(const FGameplayTag InputTag);
	void AbilityInputTagReleased(const FGameplayTag InputTag);
	void AbilityInputTagHeld(const FGameplayTag InputTag);

private:
	void                        AddDefaultMappingContext() const;
	UP16AbilitySystemComponent* GetAbilitySystemComponent();

	bool GetIsLMB(const FGameplayTag InputTag) const;

	static void                ToggleHighlight(AActor* Target, const bool bOn);
	static EP16TargetingStatus GetTargetingStatus(AActor* Target);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<USplineComponent> Spline = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputMappingContext> MappingContext = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> MoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> ShiftAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	UP16InputConfig* InputConfig = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Movement")
	float AutoRunAcceptRadius = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Movement")
	TObjectPtr<UNiagaraSystem> ClickNiagara = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Damage")
	TSubclassOf<UP16DamageTextComponent> DamageTextComponentClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Pointer")
	TSubclassOf<AP16MagicCircle> MagicCircleClass = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UP16AbilitySystemComponent> AbilitySystemComponent = {};
	UPROPERTY()
	APawn* ControlledPawn = nullptr;
	UPROPERTY()
	TObjectPtr<AP16MagicCircle> MagicCircle = nullptr;

	FHitResult          CursorHit         = {};
	FVector             CachedDestination = {};
	float               FollowTime        = 0.f;
	float               ClickThreshold    = 0.5f;
	bool                bAutoRunning      = false;
	bool                bShiftKeyDown     = false;
	EP16TargetingStatus TargetingStatus   = EP16TargetingStatus::None;
	UPROPERTY()
	AActor* LastTickActor = nullptr;
};
