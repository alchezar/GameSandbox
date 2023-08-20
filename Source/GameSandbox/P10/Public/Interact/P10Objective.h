// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P10Objective.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class USphereComponent;

UCLASS()
class GAMESANDBOX_API AP10Objective : public AActor
{
	GENERATED_BODY()

public:
	AP10Objective();
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	virtual void BeginPlay() override;
	void PlayEffect();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USphereComponent* SphereComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Effect")
	UNiagaraSystem* PickupEffect;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Effect")
	USoundBase* PickupSound;

private:
	UPROPERTY()
	UNiagaraComponent* NiagaraComponent;
};
