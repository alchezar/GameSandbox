// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "P12/Public/Util/P12CoreTypes.h"
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
class GAMESANDBOX_API AP12Turret : public APawn
{
	GENERATED_BODY()

public:
	AP12Turret();
	virtual void Tick(const float DeltaTime) override;
	virtual FVector GetPawnViewLocation() const override;
	virtual FRotator GetViewRotation() const override;
	void SetCurrentTarget(AActor* NewTarget);
	
private:
	void SetCurrentTurretState(const EP12TurretState NewState);
	void SearchingMovement(const float DeltaTime);
	void FiringMovement(const float DeltaTime);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	UNiagaraSystem* MuzzleNiagara = nullptr;
	
private:
	EP12TurretState TurretState = EP12TurretState::Searching;
	UPROPERTY()
	AActor* CurrentTarget = nullptr;
};
