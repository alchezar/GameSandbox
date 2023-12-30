// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P13/Public/Library/P13Types.h"
#include "P13Weapon.generated.h"

class UArrowComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FP13OnWeaponFireSignature, UAnimMontage* /* CharFireMontage */)
DECLARE_MULTICAST_DELEGATE_TwoParams(FP13OnWeaponReloadSignature, const bool /* bStart */,  UAnimMontage* /* CharFireMontage */)

UCLASS()
class GAMESANDBOX_API AP13Weapon : public AActor
{
	GENERATED_BODY()

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                Super                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	AP13Weapon();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(const float DeltaTime) override;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                 This                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	FORCEINLINE FP13WeaponInfo* GetWeaponInfo() const { return WeaponSettings; }
	FVector GetShootLocation() const;
	int32 GetWeaponRound();
	void WeaponInit(FP13WeaponInfo* WeaponInfo, const EP13MovementState NewState, const USkeletalMeshComponent* Mesh);
	void UpdateWeaponState(const EP13MovementState NewState);
	void SetTargetLocation(const FVector& TargetLocation);
	void SetFireState(const bool bFiring);
	void TryReloadForce();

private:
	bool CheckWeaponCanFire();
	void Fire();
	void SpawnProjectile() const;
	FVector GetFinalDirection() const;
	void UpdateDispersion(const bool bRest = false);
	void DisperseReducing();
	void InitReload();
	void FinishReload();
	void SpawnEffectsAtLocation(USoundBase* SoundBase, UNiagaraSystem* NiagaraSystem, const FVector& Location) const;
	float PlayAnimMontage(UAnimMontage* AnimMontage, const float InPlayRate = 1, const FName StartSectionName = NAME_None);
	void StopAnimMontage(const UAnimMontage* AnimMontage);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	FP13OnWeaponFireSignature OnWeaponFire;
	FP13OnWeaponReloadSignature OnWeaponReload;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USceneComponent* SceneComponent = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USkeletalMeshComponent* MeshWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UArrowComponent* ShootLocation = nullptr;

	UPROPERTY(EditAnywhere, Category = "C++ | Fire")
	FP13WeaponDynamicInfo WeaponCurrentSettings;
	
private:
	FTimerHandle FireTimer;
	FTimerHandle DisperseTimer;
	FP13WeaponInfo* WeaponSettings;
	double LastShotTime = 0.0;
	bool bReloading = false;
	FVector ShotTargetLocation = FVector::ZeroVector;
	float DispersionAngle = 0.f;
	FP13DispersionType CurrentDispersion;
};
