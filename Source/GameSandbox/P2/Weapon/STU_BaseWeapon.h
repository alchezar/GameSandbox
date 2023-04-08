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

	virtual void Fire();

protected:
	virtual void BeginPlay() override;

	void MakeShot();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapone")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapone")
	FName SocketName = "MuzzleSocket";
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapone")
	float TraceMaxDistance = 5000.f;

private:
	APlayerController* GetPlayerController() const;
	FVector            GetMuzzleSocketLocation() const;
	bool               GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
	bool               GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	void               MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;

	;
};
