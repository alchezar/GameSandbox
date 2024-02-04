// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P13/Public/Intearface/P13StateEffectInterface.h"
#include "P13/Public/Library/P13Types.h"
#include "P13CharacterBase.generated.h"

class UP13LegAlignmentComponent;
class UP13DamageDisplayComponent;
class UP13CharacterAttributesComponent;
class UP13InventoryComponent;
class AP13Weapon;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class GAMESANDBOX_API AP13CharacterBase : public ACharacter, public IP13StateEffectInterface
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP13CharacterBase();
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void PossessedBy(AController* NewController) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void UnPossessed() override;

	/* ----------------------------- Interface ----------------------------- */
public:
	virtual EPhysicalSurface GetSurfaceType() override;
	virtual bool GetCanApplyStateEffect(const TSubclassOf<UP13StateEffect> StateEffectClass = nullptr) const override;
	virtual void AddActiveStateEffect(UP13StateEffect* StateEffect) override;
	virtual void RemoveInactiveStateEffect(UP13StateEffect* InactiveStateEffect) override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE bool GetIsDead() const { return bDead; }
	FORCEINLINE EP13MovementState GetMovementState() const { return MovementState; }
	FORCEINLINE EP13MovementState GetPreviousMovementState() const { return PreviousMovementState; }
	FORCEINLINE AP13Weapon* GetCachedWeapon() const { return CachedWeapon.Get(); }
	FORCEINLINE UP13InventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	FORCEINLINE UP13LegAlignmentComponent* GetLegAlignmentComponent() const { return LegAlignmentComponent; }
	FORCEINLINE EP13AmmoType GetCurrentWeaponType() const { return CurrentWeaponType; }
	FORCEINLINE FLinearColor GetTrueColor() const { return TrueColor; }
	virtual FVector GetLookAtCursorDirection() const;
	float GetHealthReserve() const;
	void SavePreviousMovementState() { PreviousMovementState = MovementState; }

	bool TryCreateDynamicMeshMaterials();
	void UpdateDynamicMeshMaterials(const FLinearColor NewColor);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdatePlayerColor(const FLinearColor NewColor);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayReadyAnimation();

protected:
	void UpdateCharacter() const;
	void ChangeMovementState(const EP13MovementState NewMovementState);
	virtual void UpdateInventoryAfterRespawn();

	void OnHealthChangedHandle(const float NewHealth, const float LastDamage, const float HealthAlpha);
	void OnShieldChangedHandle(const float NewShield, const float LastDamage, const float ShieldAlpha);
	virtual void OnDeathHandle(AController* Causer);

	virtual void InitWeapon(const FP13WeaponSlot& NewWeaponSlot, const int32 CurrentIndex);
	void DropWeapon(const bool bTakeNext);
	void PullTrigger(const bool bStart) const;
	bool TryReloadWeapon() const;
	bool TryTakeNextWeapon(const bool bNext) const;
	bool CheckCharacterCanFire() const;

	void OnWeaponFiredHandle(UAnimMontage* CharFireAnim, const int32 CurrentRound);
	void OnWeaponReloadInitHandle(const int32 OldRoundNum);
	void OnWeaponReloadStartHandle(UAnimMontage* CharReloadAnim, const int32 WeaponIndex, const float ReloadingTime);
	virtual void OnWeaponReloadFinishHandle(const int32 RoundNum, const int32 WeaponIndex, const bool bSuccess);

	virtual void TryLoadSavedColor(AController* NewController);

private:
	void CreateBaseComponents();
	void TakeStateEffectFromRadialDamage(FDamageEvent const& DamageEvent, AActor* DamageCauser);
	void PlayTakeDamageEffect(const AActor* DamageCauser);

	/* ------------------------------ Network ------------------------------ */
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION(Server, Reliable)
	void Server_ChangeMovementState(const EP13MovementState NewMovementState);
	UFUNCTION()
	void OnRep_MovementState();
	UFUNCTION(Server, Reliable)
	void Server_InitWeapon(const FP13WeaponSlot& NewWeaponSlot, const int32 CurrentIndex);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_InitWeapon(const FP13WeaponSlot& NewWeaponSlot, const int32 CurrentIndex);

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UP13InventoryComponent* InventoryComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UP13CharacterAttributesComponent* AttributesComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UP13DamageDisplayComponent* DamageDisplayComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UP13LegAlignmentComponent* LegAlignmentComponent;

	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	UAnimMontage* TakeDamageAnim = nullptr;

	UPROPERTY(ReplicatedUsing = "OnRep_MovementState", EditAnywhere, Category = "C++ | Movement")
	EP13MovementState MovementState = EP13MovementState::Run;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement")
	FP13MovementSpeed MovementSpeed;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement")
	float RotationRate = 10.f;

	UPROPERTY(EditAnywhere, Category = "C++ | Preview")
	UAnimMontage* ReadyMontage = nullptr;

	TWeakObjectPtr<AController> ControllerCached;
	UPROPERTY(Replicated)
	TWeakObjectPtr<AP13Weapon> CachedWeapon;
	UPROPERTY(Replicated)
	EP13AmmoType CurrentWeaponType = EP13AmmoType::Default;
	UPROPERTY(Replicated)
	FLinearColor TrueColor = FLinearColor::White;

private:
	bool bDead = false;
	EP13MovementState PreviousMovementState = EP13MovementState::Walk;
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;
	UPROPERTY()
	TArray<UP13StateEffect*> ActiveStateEffects;
};
