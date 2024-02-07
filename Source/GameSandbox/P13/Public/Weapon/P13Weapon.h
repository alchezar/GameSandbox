// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P13/Public/Library/P13Types.h"
#include "P13Weapon.generated.h"

class UArrowComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FP13OnWeaponFireSignature, UAnimMontage* /* CharFireMontage */, const int32 /*CurrentRound*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FP13OnWeaponReloadInitSignature, int32 /*OldRoundNum*/)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FP13OnWeaponReloadStartSignature, UAnimMontage* /* CharFireMontage */, int32 /*WeaponIndex*/, float /*ReloadingTime*/)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FP13OnWeaponReloadFinishSignature, int32 /*NewRoundNum*/, int32/*WeaponIndex*/, const bool /*bSuccess*/)

UCLASS()
class GAMESANDBOX_API AP13Weapon : public AActor
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13Weapon();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(const float DeltaTime) override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE FP13WeaponInfo* GetWeaponInfo() const { return WeaponSettings; }
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return MeshWeapon; };
	FORCEINLINE FP13WeaponDynamicInfo GetDynamicInfo() const { return WeaponCurrentSettings; }
	FORCEINLINE int32 GetWeaponIndex() const { return WeaponIndex; }
	FVector GetShootLocation() const;
	float GetShootRate() const;
	void WeaponInit(FP13WeaponInfo* WeaponInfo, const EP13MovementState NewState, const int32 NewWeaponIndex, const FP13WeaponDynamicInfo* DynamicInfo = nullptr);
	void UpdateWeaponState(const EP13MovementState NewState);
	void UpdateWeaponDynamicInfo(const FP13WeaponDynamicInfo* DynamicInfo);
	void SetTargetLocation(const FVector& TargetLocation);
	void SetFireState(const bool bFiring);
	bool TryReload();
	void AbortReloading();
	void PlayWeaponReload();
	void SetMaxAvailableRound(const int32 NewMaxRound = -1);

private:
	bool CheckWeaponCanFire() const;
	bool CheckWeaponCanReload();
	void Fire();
	void MakeShot();
	void SpawnProjectile();
	FVector GetFinalDirection() const;
	void UpdateDispersion(const bool bRest = false);
	void DisperseReducing();
	void StartReload();
	void FinishReload(const bool bSuccess = true);
	void SpawnEffectsAtLocation(USoundBase* SoundBase, UNiagaraSystem* NiagaraSystem, const FVector& Location) const;
	float PlayAnimMontage(UAnimMontage* AnimMontage, const float InPlayRate = 1, const FName StartSectionName = NAME_None);
	void StopAnimMontage(const UAnimMontage* AnimMontage);

	/* ------------------------------ Network ------------------------------ */
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(Server, Reliable)
	void Server_Fire();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ShotEffect(USoundBase* FireSound, UNiagaraSystem* FireEffect, UAnimMontage* FireAnim);
	UFUNCTION(Server, Reliable)
	void Server_InitReload();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAnimMontage(UAnimMontage* Montage);
	UFUNCTION(Server, Reliable)
	void Server_UpdateWeaponState(const EP13MovementState NewState);

	/* ----------------------------- Variables ----------------------------- */
public:
	FP13OnWeaponFireSignature OnWeaponFire;
	FP13OnWeaponReloadInitSignature OnWeaponReloadInit;
	FP13OnWeaponReloadStartSignature OnWeaponReloadStart;
	FP13OnWeaponReloadFinishSignature OnWeaponReloadFinish;

protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USceneComponent* SceneComponent = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USkeletalMeshComponent* MeshWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UArrowComponent* ShootLocation = nullptr;

	UPROPERTY(Replicated, EditAnywhere, Category = "C++ | Fire")
	FP13WeaponDynamicInfo WeaponCurrentSettings;

private:
	FTimerHandle FireTimer;
	FTimerHandle DisperseTimer;
	FTimerHandle ReloadTimer;
	FP13WeaponInfo* WeaponSettings;
	double LastShotTime = 0.0;
	bool bReloading = false;
	FVector ShotTargetLocation = FVector::ZeroVector;
	UPROPERTY(Replicated)
	float DispersionAngle = 0.f;
	FP13DispersionType CurrentDispersion;
	int32 MaxAvailableRound = 0;
	bool bTriggerPulled = false;
	UPROPERTY(Replicated)
	int32 WeaponIndex = 0;
};
