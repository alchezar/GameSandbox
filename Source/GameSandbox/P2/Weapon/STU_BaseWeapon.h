// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STU_BaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class GAMESANDBOX_API ASTU_BaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASTU_BaseWeapon();

	virtual void StartFire();
	virtual void StopFire();

protected:
	virtual void BeginPlay() override;

	virtual void       MakeShot();
	APlayerController* GetPlayerController() const;
	FVector            GetMuzzleSocketLocation() const;
	virtual bool       GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
	bool               GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	virtual void       MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
	virtual void       MakeDamage(const FHitResult& HitResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapone")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapone")
	FName SocketName = "MuzzleSocket";
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapone", meta = (Units = "cm"))
	float TraceMaxDistance = 5000.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapone")
	float DamageAmount = 10.f;

private:
	FTimerHandle ShotTimer;
	;
};
