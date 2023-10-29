// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12/Public/Actor/Equipment/P12EquipableItem.h"
#include "P12ThrowableItem.generated.h"

class AP12Projectile;

UCLASS(Blueprintable)
class GAMESANDBOX_API AP12ThrowableItem : public AP12EquipableItem
{
	GENERATED_BODY()

public:
	AP12ThrowableItem();
	FORCEINLINE float GetGravityCompensationAngle() const { return GravityCompensationAngle; }
	void TakeThrowable();
	void Throw();

protected:
	virtual void BeginPlay() override;
	virtual void InitAnimNotify() override;
	void OnThrowableTakenHandle(USkeletalMeshComponent* SkeletalMeshComponent);
	void OnThrowableLaunchedHandle(USkeletalMeshComponent* SkeletalMeshComponent);
	void OnEquipmentStateChangedHandle(const bool bEquipped);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UStaticMeshComponent* ProjectileBillet;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Throw")
	TSubclassOf<AP12Projectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "C++ | Throw")
	float GravityCompensationAngle = 10.f;
};
