// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P13/Public/Library/P13Types.h"
#include "P13Weapon.generated.h"

class UArrowComponent;

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
	virtual void Tick(float DeltaTime) override;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                 This                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	void WeaponInit(FP13WeaponInfo* WeaponInfo);
	void UpdateWeaponState(EP13MovementState NewState);
	void SetFireState(const bool bFiring);

private:
	void Fire();
	bool CheckWeaponCanFire();
	void SpawnProjectile() const;
	void ChangeDispersion();

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USceneComponent* SceneComponent = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USkeletalMeshComponent* MeshWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UArrowComponent* ShootLocation = nullptr;

	UPROPERTY(EditAnywhere, Category = "C++ | Fire")
	FP13WeaponDynamicInfo WeaponDynamicSettings;
	
private:
	FTimerHandle FireTimer;
	FP13WeaponInfo* WeaponSettings;
	double LastShotTime = 0.f;
};
