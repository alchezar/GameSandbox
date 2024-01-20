// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P13/Public/Intearface/P13StateEffectInterface.h"
#include "P13/Public/Library/P13Types.h"

#include "P13CharacterBase.generated.h"

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

	/* ------------------------------- Super ------------------------------- */
public:
	AP13CharacterBase();
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void UnPossessed() override;
	
protected:
	virtual void BeginPlay() override;

	/* ----------------------------- Interface ----------------------------- */
public:
	virtual EPhysicalSurface GetSurfaceType() override;
	virtual bool GetCanApplyStateEffect(const TSubclassOf<UP13StateEffect> StateEffectClass = nullptr) const override;
	virtual void AddActiveStateEffect(UP13StateEffect* StateEffect) override;
	virtual void RemoveInactiveStateEffect(UP13StateEffect* InactiveStateEffect) override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE bool GetIsDead() const { return bDead; }
	FORCEINLINE float GetIKLeftLegOffset() const { return IKLeftLegOffset; }
	FORCEINLINE float GetIKRightLegOffset() const { return IKRightLegOffset; }
	FORCEINLINE float GetIKHipOffset() const { return IKHitOffset; }
	FORCEINLINE EP13MovementState GetMovementState() const { return MovementState; }
	FORCEINLINE EP13MovementState GetPreviousMovementState() const { return PreviousMovementState; }
	FORCEINLINE AP13Weapon* GetCachedWeapon() const { return CachedWeapon.Get(); }
	FORCEINLINE UP13InventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	FORCEINLINE EP13AmmoType GetCurrentWeaponType() const { return CurrentWeaponType; }
	virtual FVector GetLookAtCursorDirection() const;
	void SavePreviousMovementState() { PreviousMovementState = MovementState; }

protected:
	void UpdateCharacter() const;
	void ChangeMovementState(const EP13MovementState NewMovementState);
	void UpdateInventoryAfterRespawn() const;

	void OnHealthChangedHandle(const float NewHealth, const float LastDamage, const float HealthAlpha);
	void OnShieldChangedHandle(const float NewShield, const float LastDamage, const float ShieldAlpha);
	virtual void OnDeathHandle(AController* Causer);

	void InitWeapon(const FP13WeaponSlot& NewWeaponSlot, const int32 CurrentIndex);
	void DropWeapon(const bool bTakeNext);
	bool CheckCharacterCanFire() const;
	void OnWeaponFiredHandle(UAnimMontage* CharFireAnim, const int32 CurrentRound);
	void OnWeaponReloadInitHandle(const int32 OldRoundNum);
	void OnWeaponReloadStartHandle(UAnimMontage* CharReloadAnim, const int32 WeaponIndex, const float ReloadingTime);
	void OnWeaponReloadFinishHandle(const int32 RoundNum, const int32 WeaponIndex, const bool bSuccess);

private:
	void CreateBaseComponents();
	/* Leg alignment. */
	float GetIKSocketOffset(const FName& VirtualBoneName, const float TraceHalfDistance = 50.f, const float FromBoneToBottom = 10.f) const;
	void LegsIKFloorAlignment();
	void CreateDynamicMeshMaterials();
	void UpdateMeshMaterial(const float HealthAlpha);
	void TakeStateEffectFromRadialDamage(FDamageEvent const& DamageEvent, AActor* DamageCauser);
	void PlayTakeDamageEffect(const AActor* DamageCauser);

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UP13InventoryComponent* InventoryComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UP13CharacterAttributesComponent* AttributesComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UP13DamageDisplayComponent* DamageDisplayComponent;

	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	UCurveLinearColor* ColorOverLife;
	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	UAnimMontage* TakeDamageAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "C++ | Movement")
	EP13MovementState MovementState = EP13MovementState::Run;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement")
	FP13MovementSpeed MovementSpeed;
	UPROPERTY(EditAnywhere, Category = "C++ | Movement")
	float RotationRate = 10.f;

	TWeakObjectPtr<AController> ControllerCached;
	TWeakObjectPtr<AP13Weapon> CachedWeapon;
	EP13AmmoType CurrentWeaponType = EP13AmmoType::Default;

private:
	bool bDead = false;
	float IKLeftLegOffset = 0.f;
	float IKRightLegOffset = 0.f;
	float IKHitOffset = 0.f;
	EP13MovementState PreviousMovementState = EP13MovementState::Walk;
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;
	UPROPERTY()
	TArray<UP13StateEffect*> ActiveStateEffects;
};
