// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "Utils/P15Types.h"
#include "P15Character.generated.h"

class UCameraComponent;
class UGameplayAbility;
class UInputAction;
class UInputMappingContext;
class UP15AttributeSet;
class USpringArmComponent;
struct FInputActionValue;

UCLASS()
class PROJECT15_API AP15Character : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP15Character();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/* ----------------------------- Interface ----------------------------- */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void                     GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	/* ------------------------------- This -------------------------------- */
public:
	_NODISCARD TObjectPtr<UAbilitySystemComponent> GetAbilitySystemComp() { return AbilitySystemComp; }
	_NODISCARD TObjectPtr<UCameraComponent>        GetPlayerEye() const { return PlayerEye; }
	_NODISCARD TObjectPtr<UP15AttributeSet>        GetAttributeSet() const { return AttributeSet; }
	_NODISCARD bool                                GetIsMoving() const { return bMovingInput && bAllowMoving; }
	_NODISCARD bool                                GetIsDead() const { return bDead; }
	_NODISCARD uint8                               GetTeamID() const { return TeamID; }

	void SetAllowMoving(const bool bAllow) { bAllowMoving = bAllow; }

	UFUNCTION(BlueprintCallable, Category = "C++")
	bool GetIsHostile(const AP15Character* Other) const;
	void AcquireAllAbilities();
	void AddGameplayTag(const FGameplayTag TagToAdd) const;
	void RemoveGameplayTag(const FGameplayTag TagToRemove) const;
	void ToggleGameplayTag(const FGameplayTag TagToToggle, const bool bAdd) const;

protected:
	void MoveInput(const FInputActionValue& InputValue);
	void ChangeMovementState(const bool bStart);
	void LookInput(const FInputActionValue& InputValue);
	void RunInput(const bool bRun);
	void CrouchInput();
	UFUNCTION(BlueprintCallable)
	void PushInput();
	void AttackInput(const bool bStart);
	void RegenInput();
	void OnHealthChangedCallback(const float NewHealthPercentage);
	void OnManaChangedCallback(const float NewManaPercentage);
	void OnStrengthChangedCallback(const float NewStrengthPercentage);

private:
	void AddDefaultMappingContext() const;
	void ChangeWalkSpeedSmoothly(const float DeltaTime);
	void UpdateCameraBoomOffsetSmoothly(const float DeltaTime);
	void AutoDetermineTeamID();

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComp = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<USpringArmComponent> CameraBoom = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UCameraComponent> PlayerEye = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputMappingContext> InputContext = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> MoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> LookAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> JumpAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> RunAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> CrouchAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> AttackAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> AimAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> RegenAction = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Ability")
	TObjectPtr<UP15AttributeSet> AttributeSet = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	TSubclassOf<UGameplayAbility> PushAbility = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	TSubclassOf<UGameplayAbility> MeleeAbility = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	TSubclassOf<UGameplayAbility> DeadAbility = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	TSubclassOf<UGameplayAbility> RegenAbility = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Tag")
	FGameplayTag FullHealthTag = FGameplayTag::RequestGameplayTag("p15.health.full");

private:
	FP15SmoothChangeData SpeedChangeData        = {};
	FP15SmoothChangeData CameraOffsetChangeData = {};

	bool bMovingInput = false;
	bool bAllowMoving = true;

	double MaxCrouchOffset = 0.0;
	bool   bDead           = false;
	uint8  TeamID          = 255;

	TArray<TSubclassOf<UGameplayAbility>> AllAbilities = {};
};
