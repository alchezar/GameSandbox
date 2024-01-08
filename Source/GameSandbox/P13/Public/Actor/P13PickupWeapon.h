// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P13PickupAmmo.h"
#include "P13PickupWeapon.generated.h"

UCLASS()
class GAMESANDBOX_API AP13PickupWeapon : public AP13PickupAmmo
{
	GENERATED_BODY()

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                Super                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	AP13PickupWeapon();

protected:
	virtual void BeginPlay() override;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                 This                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	void InitDrop(const FP13WeaponDrop* DropWeaponInfo);
	
protected:
	virtual void OnCollisionBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Weapon", meta = (EditCondition = "PickupType == EP13PickupType::Weapon"))
	FP13WeaponSlot WeaponSlot;
	UPROPERTY(EditAnywhere, Category = "C++ | Weapon", meta = (EditCondition = "PickupType == EP13PickupType::Weapon"))
	float ActivationDelay = 3.f;

private:
	FTimerHandle DropTimer;
	bool bDropped = false;
};
