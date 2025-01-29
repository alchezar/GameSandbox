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
	UFUNCTION(BlueprintCallable, Category = "C++")
	void AcquireAbility(const TSubclassOf<UGameplayAbility>& AbilityToAcquire);

protected:
	void MoveInput(const FInputActionValue& InputValue);
	void LookInput(const FInputActionValue& InputValue);
	void RunInput(const bool bRun);
	void CrouchInput();
	void AttackInput(const bool bStart);

private:
	void AddDefaultMappingContext() const;
	void ChangeWalkSpeedSmoothly(const float DeltaTime);
	void UpdateCameraBoomOffsetSmoothly(const float DeltaTime);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Ability")
	TObjectPtr<UP15AttributeSet> AttributeSet = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	TSubclassOf<UGameplayAbility> MeleeAbility = nullptr;

private:
	FP15SmoothChangeData SpeedChangeData        = {};
	FP15SmoothChangeData CameraOffsetChangeData = {};
	double               MaxCrouchOffset        = 0.0;
};
