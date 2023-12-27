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
	void SetFireState(bool bFiring);
	void Fire();

private:
	bool CheckWeaponCanFire();
	void SpawnProjectile(TSubclassOf<AP13ProjectileDefault> ProjectileClass);

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
	FP13WeaponInfo WeaponSettings;

private:
	FTimerHandle FireTimer;
};
