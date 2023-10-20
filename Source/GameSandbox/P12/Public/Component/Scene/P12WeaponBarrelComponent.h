// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "P12WeaponBarrelComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP12WeaponBarrelComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UP12WeaponBarrelComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
};
