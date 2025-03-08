// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "P16PlayerController.generated.h"

class USplineComponent;
class UP16AbilitySystemComponent;
struct FGameplayTag;
class UP16InputConfig;
class IP16InterfaceEnemy;
class AP16Character;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class PROJECT16_API AP16PlayerController : public APlayerController
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP16PlayerController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaSeconds) override;
	virtual void SetupInputComponent() override;

	/* ------------------------------- This -------------------------------- */
protected:
	void MoveInputCallback(const FInputActionValue& InputValue);
	void CursorTrace();
	void AutoRun();

	void AbilityInputTagPressed(const FGameplayTag InputTag);
	void AbilityInputTagReleased(const FGameplayTag InputTag);
	void AbilityInputTagHeld(const FGameplayTag InputTag);

private:
	void                        AddDefaultMappingContext() const;
	UP16AbilitySystemComponent* GetAbilitySystemComponent();

	bool GetIsLMB(const FGameplayTag InputTag) const;

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<USplineComponent> Spline = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputMappingContext> MappingContext = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> MoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	UP16InputConfig* InputConfig = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Movement")
	float AutoRunAcceptRadius = 50.f;

private:
	TScriptInterface<IP16InterfaceEnemy> LastTickEnemy = {};
	UPROPERTY()
	TObjectPtr<UP16AbilitySystemComponent> AbilitySystemComponent = {};
	UPROPERTY()
	APawn* ControlledPawn = nullptr;

	FHitResult CursorHit         = {};
	FVector    CachedDestination = {};
	float      FollowTime        = 0.f;
	float      ClickThreshold    = 0.5f;
	bool       bAutoRunning      = false;
	bool       bTargeting        = false;
};
