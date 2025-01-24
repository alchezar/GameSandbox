// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Util/P12CoreTypes.h"
#include "P12Turret.generated.h"

class UNiagaraSystem;
class UP12WeaponBarrelComponent;

UENUM(BlueprintType)
enum class EP12TurretState : uint8
{
	Searching,
	Firing
};

UCLASS()
class PROJECT12_API AP12Turret : public APawn
{
	GENERATED_BODY()

public:
	AP12Turret();
	virtual void Tick(const float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual FVector GetPawnViewLocation() const override;
	virtual FRotator GetViewRotation() const override;
	virtual void PossessedBy(AController* NewController) override;
	void SetCurrentTarget(AActor* NewTarget);
	void OnCurrentTargetSet();

private:
	void SetCurrentTurretState(const EP12TurretState NewState);
	void SearchingMovement(const float DeltaTime);
	void FiringMovement(const float DeltaTime);
	float GetFireInterval() const;
	void MakeShot();
	void BindOnTargetHealthChanged(AActor* Target, const bool bBind);
	void OnTargetHealthChangedHandle(float Health, float MaxHealth);

	UFUNCTION()
	void OnRep_CurrentTarget();

public:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentTarget)
	AActor* CurrentTarget = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	USceneComponent* TurretBaseComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	USceneComponent* TurretBarrelComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Component")
	UP12WeaponBarrelComponent* BarrelComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Turret", meta = (ClampMin = 0.f, UIMin = 0.f, Units = "deg"))
	float BaseSearchingRotationRate = 60.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Turret", meta = (ClampMin = 0.f, UIMin = 0.f, Units = "deg"))
	float BaseFiringInterpSpeed = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Turret", meta = (ClampMin = 0.f, UIMin = 0.f, Units = "deg"))
	float BarrelPitchRotationRate = 60.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Turret")
	FP12Diapason BarrelPitchAngle = {-30.f, 30.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Fire", meta = (ClampMin = 1.f, UIMin = 1.f))
	float RateOfFire = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Fire")
	float BulletSpread = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Fire")
	float FireDelayTime = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	UNiagaraSystem* MuzzleNiagara = nullptr;

	UPROPERTY(EditAnywhere, Category = "C++ | Team")
	EP12Teams Team = EP12Teams::Enemy;

private:
	EP12TurretState TurretState = EP12TurretState::Searching;
	FTimerHandle ShotTimer;
};
