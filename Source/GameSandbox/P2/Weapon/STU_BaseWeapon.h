// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STU_BaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class GAMESANDBOX_API ASTU_BaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASTU_BaseWeapon();

protected:
	virtual void BeginPlay() override;

#pragma region Component

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapone")
	USkeletalMeshComponent* WeaponMesh;

#pragma endregion // Component
};
