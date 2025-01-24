// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Library/P13Types.h"

#include "P13PickupActor.generated.h"

class UNiagaraSystem;
class IP13PickupInterface;
class USphereComponent;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              PickingUp Base                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UCLASS(Abstract, NotBlueprintable)
class PROJECT13_API AP13PickingUpBase : public AActor
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13PickingUpBase();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	/* ------------------------------- This -------------------------------- */
public:
	virtual void OnPickupSuccess();

protected:
	UFUNCTION()
	virtual void OnCollisionBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void ActivateParticles();
	virtual void BePickedUpOnCollisionBeginOverlap(AActor* Picker);

private:
	void CreateComponents();

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USphereComponent* Collision;

	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	UNiagaraSystem* WaitEffect;
	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	UNiagaraSystem* PickupEffect;
	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	FLinearColor EffectColor = FColor::White;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                               PickingUp Aid                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UCLASS(Blueprintable)
class PROJECT13_API AP13PickingUpAid : public AP13PickingUpBase
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13PickingUpAid();

	/* ------------------------------- This -------------------------------- */
protected:
	virtual void BePickedUpOnCollisionBeginOverlap(AActor* Picker) override;

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	float HealthAid = 50.f;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              PickingUp Ammo                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UCLASS(Blueprintable)
class PROJECT13_API AP13PickingUpAmmo : public AP13PickingUpBase
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13PickingUpAmmo() {}

protected:
	virtual void BeginPlay() override;

	/* ------------------------------- This -------------------------------- */
protected:
	virtual void BePickedUpOnCollisionBeginOverlap(AActor* Picker) override;

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Ammo")
	FP13AmmoSlot AmmoSlot = {EP13AmmoType::Default, 0, 0};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                             PickingUp Weapon                              *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UCLASS()
class PROJECT13_API AP13PickingUpWeapon : public AP13PickingUpAmmo
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13PickingUpWeapon() {}

protected:
	virtual void BeginPlay() override;

	/* ------------------------------- This -------------------------------- */
public:
	void InitDrop(const FP13WeaponDrop* DropWeaponInfo);

protected:
	virtual void ActivateParticles() override;
	virtual void BePickedUpOnCollisionBeginOverlap(AActor* Picker) override;

private:
	void MakePickableAfterDrop();

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Weapon", meta = (EditCondition = "PickupType == EP13PickupType::Weapon"))
	FP13WeaponSlot WeaponSlot;
	UPROPERTY(EditAnywhere, Category = "C++ | Weapon", meta = (EditCondition = "PickupType == EP13PickupType::Weapon"))
	float ActivationDelay = 3.f;

private:
	FTimerHandle DropTimer;
	bool bDropped = false;
};
