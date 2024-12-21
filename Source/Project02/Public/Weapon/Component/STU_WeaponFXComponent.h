// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Game/STU_CoreTypes.h"
#include "STU_WeaponFXComponent.generated.h"

class UNiagaraSystem;
class UPhysicalMaterial;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT02_API USTU_WeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USTU_WeaponFXComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void PlayImpactFX(const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Impact FX")
	FImpactData DefaultImpactData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | Impact FX")
	TMap<UPhysicalMaterial*, FImpactData> ImpactDataMap;
};
