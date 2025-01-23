// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P14Turret.generated.h"

class AP14Projectile;

UCLASS(Abstract)
class PROJECT14_API AP14Turret : public AActor
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	AP14Turret();

protected:
	virtual void BeginPlay() override;

	/* ------------------------------- This -------------------------------- */
protected:
	void OnFire();

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<USceneComponent> SceneRoot = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UStaticMeshComponent> TurretMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<AP14Projectile> ProjectileClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	int32 AmmoCount = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (Units = s))
	float FireFrequency = 1.f;

private:
	UPROPERTY()
	FTimerHandle FireTimer;
};
