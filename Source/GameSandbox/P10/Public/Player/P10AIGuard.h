// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P10AIGuard.generated.h"

class ATargetPoint;
class UWidgetComponent;
class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EP10AIGuardState : uint8
{
	Idle,
	Suspicious,
	Alert
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatusChangedSignature, EP10AIGuardState)

UCLASS()
class GAMESANDBOX_API AP10AIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	AP10AIGuard();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE EP10AIGuardState GetGuardState() const { return GuardState; }
	void MoveToTarget();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSeePawnHandle(APawn* Pawn);
	UFUNCTION()
	void OnHearNoiseHandle(APawn* NoiseInstigator, const FVector& Location, float Volume);

private:
	void OrientGuardHandle();
	void ResetRotationHandle();
	void ChangeGuardState(const EP10AIGuardState NewState);

public:	
	FOnStatusChangedSignature OnStatusChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UPawnSensingComponent* PawnSensing;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UWidgetComponent* StatusWidget;
	
	UPROPERTY(EditAnywhere, Category = "C++ | Patrol")
	bool bPatrol = false;
	UPROPERTY(EditAnywhere, Category = "C++ | Patrol")
	TArray<ATargetPoint*> Targets;
	
private:
	FRotator OriginalRotation;
	FRotator TargetRotation;
	FTimerHandle DistractionTimer;
	FTimerHandle ResetTimer;
	EP10AIGuardState GuardState = EP10AIGuardState::Idle;
	int32 CurrentTargetsElement = 0;
};
