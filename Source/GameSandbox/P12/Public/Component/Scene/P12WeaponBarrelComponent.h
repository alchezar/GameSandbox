// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "P12WeaponBarrelComponent.generated.h"

class UNiagaraSystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP12WeaponBarrelComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UP12WeaponBarrelComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Shot(const FVector& ShotStart, const FVector& ShotDirection, AController* Instigator);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Barrel")
	float FiringRange = 5000.f;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Barrel")
	float DamageAmount = 20.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Alignment")
	UNiagaraSystem* TraceNiagara = nullptr;
};
