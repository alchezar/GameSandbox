// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "P13StateEffect.generated.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                          State Effect Base Class                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS(Blueprintable)
class GAMESANDBOX_API UP13StateEffect : public UObject
{
	GENERATED_BODY()

public:
	UP13StateEffect();

public:
	FORCEINLINE TArray<TEnumAsByte<EPhysicalSurface>> GetPossibleInteractSurfaces() const { return PossibleInteractSurfaces; }
	FORCEINLINE bool GetIsStackable() const { return bStackable; }
	virtual void InitObject(AActor* NewActor);
	virtual void DestroyObject();

public:

protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TArray<TEnumAsByte<EPhysicalSurface>> PossibleInteractSurfaces;
	UPROPERTY(EditAnywhere, Category = "C++")
	bool bStackable = false;

	TWeakObjectPtr<AActor> CachedActor;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                       State Effect Single Execute                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UCLASS()
class UP13SingleStateEffect : public UP13StateEffect
{
	GENERATED_BODY()

public:
	virtual void InitObject(AActor* NewActor) override;
	virtual void DestroyObject() override;
	virtual void StartEffect();
	virtual void ApplyEffect();

protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	float Power = 20.f;
	UPROPERTY(EditAnywhere, Category = "C++")
	UNiagaraSystem* Particle;

	UPROPERTY()
	UNiagaraComponent* ParticleComponent;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                         State Effect Timer Execute                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UCLASS()
class UP13TimerStateEffect : public UP13SingleStateEffect
{
	GENERATED_BODY()

public:
	virtual void InitObject(AActor* NewActor) override;
	virtual void DestroyObject() override;
	virtual void StartEffect() override;
	virtual void ApplyEffect() override;

protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	float Duration = 5.f;
	UPROPERTY(EditAnywhere, Category = "C++")
	float Rate = 1.f;

private:
	FTimerHandle DurationTimer;
	FTimerHandle RateTimer;
};
