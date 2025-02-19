// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "Utils/P15Types.h"
#include "Utils/P15Utils.h"
#include "P15Character.generated.h"

class UGameplayEffect;
class UCameraComponent;
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
	virtual void PossessedBy(AController* NewController) override;

	/* ----------------------------- Interface ----------------------------- */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void                     GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	/* ------------------------------- This -------------------------------- */
public:
	_NODISCARD TObjectPtr<UAbilitySystemComponent> GetAbilitySystemComp() { return AbilitySystemComp; }
	_NODISCARD TObjectPtr<UCameraComponent>        GetPlayerEye() const { return PlayerEye; }
	_NODISCARD TObjectPtr<UP15AttributeSet>        GetAttributeSet() const { return AttributeSet; }
	_NODISCARD bool                                GetIsMoving() const { return bMovingInput; }
	_NODISCARD bool                                GetIsDead() const { return bDead; }
	_NODISCARD uint8                               GetTeamID() const { return TeamID; }

	void SetCollisionResponseToPawn(const ECollisionResponse NewResponse);
	void SetTargetingState(const bool bNewTargeting);

	UFUNCTION(BlueprintCallable, Category = "C++")
	bool GetIsHostile(const AP15Character* Other) const;
	void AddGameplayTag(const FGameplayTag TagToAdd) const;
	void RemoveGameplayTag(const FGameplayTag TagToRemove) const;
	void ToggleGameplayTag(const FGameplayTag TagToToggle, const bool bAdd) const;
	void PushCharacter(const FVector& Direction, const float Strength, const float Duration = 1.f);
	void Stun(const float Duration);

protected:
	void MoveInput(const FInputActionValue& InputValue);
	void ChangeMovementState(const bool bStart);
	void LookInput(const FInputActionValue& InputValue);
	void JumpInput(const bool bInAir);
	void RunInput(const bool bRun);
	void CrouchInput();
	UFUNCTION(BlueprintCallable)
	void PushInput();
	void AttackInput(const bool bStart);
	void RegenInput();
	void DashInput();
	void LaserInput();
	void BlastInput();
	void FireInput();
	void OnHealthChangedCallback(const float NewHealthPercentage);
	void OnManaChangedCallback(const float NewManaPercentage);
	void OnStrengthChangedCallback(const float NewStrengthPercentage);
	UFUNCTION()
	void OnCapsuleBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void AddDefaultMappingContext() const;
	void AutoDetermineTeamID();
	void AcquireAllAbilities();
	void AddAbilitiesToUI();
	void ActivateAbility(const TSubclassOf<UP15BaseAbility>& AbilityClass, const bool bImmediately = true);
	void ChangeWalkSpeedSmoothly(const float DeltaTime);
	void UpdateCameraBoomOffsetSmoothly(const float DeltaTime);
	void StartRegen();

	/* ------------------------------ Fields ------------------------------- */
public:
	FP15OnAbilityStartedSignature OnAbilityStarted;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> DashAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> LaserAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> BlastAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Input")
	TObjectPtr<UInputAction> FireAction = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Ability")
	TObjectPtr<UP15AttributeSet> AttributeSet = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	TSubclassOf<UP15BaseAbility> PushAbility = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	TSubclassOf<UP15BaseAbility> MeleeAbility = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	TSubclassOf<UP15BaseAbility> DeadAbility = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	TSubclassOf<UP15BaseAbility> RegenAbility = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	TSubclassOf<UP15BaseAbility> DashAbility = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	TSubclassOf<UP15BaseAbility> LaserAbility = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	TSubclassOf<UP15BaseAbility> BlastAbility = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Ability")
	TSubclassOf<UP15BaseAbility> FireAbility = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Regen")
	TSubclassOf<UGameplayEffect> RegenEffectClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Regen")
	float RegenDelay = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Tag")
	FGameplayTag FullHealthTag = FGameplayTag::RequestGameplayTag("p15.char.health.full");

private:
	FP15SmoothChangeData SpeedChangeData        = {};
	FP15SmoothChangeData CameraOffsetChangeData = {};

	bool bMovingInput = false;
	bool bFalling     = false;
	bool bTargeting   = false;
	bool bDead        = false;

	double MaxCrouchOffset = 0.0;
	uint8  TeamID          = 255;

	TArray<TSubclassOf<UP15BaseAbility>> AllAbilities = {};
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController = nullptr;

	FTimerHandle StunTimer;
	FTimerHandle RegenTimer;

	FActiveGameplayEffectHandle RegenEffectHandle = {};
	float                       LastAllPercentage = 1.f;
};
