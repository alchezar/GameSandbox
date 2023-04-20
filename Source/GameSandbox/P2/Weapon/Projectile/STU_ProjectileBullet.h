// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_ProjectileBase.h"
#include "STU_ProjectileBullet.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class GAMESANDBOX_API ASTU_ProjectileBullet : public ASTU_ProjectileBase
{
	GENERATED_BODY()

public:
	ASTU_ProjectileBullet();
	virtual void Tick(float DeltaTime) override;

	void SpawnTraceFX();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Bullet")
	float HeadshotMultiplier = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Bullet")
	float GravityScale = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Trace FX")
	UNiagaraSystem* TraceFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Trace FX")
	FName TraceTargetName = "TraceTarget";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Trace FX")
	double BlasterBoltLength = 400.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Trace FX")
	FName BlasterColorName = "BlasterColor";

private:
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult) override;
	void         UpdateBoltTaleOffset(FVector TaleOffset = FVector::ZeroVector) const;
	void         SetBlasterColor();

	UPROPERTY()
	UNiagaraComponent* TraceFXComponent;
	FVector            ForwardVector;

	FVector StartLocation;
};
