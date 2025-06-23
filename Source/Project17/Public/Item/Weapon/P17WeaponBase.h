// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P17WeaponBase.generated.h"

class UBoxComponent;

UCLASS()
class PROJECT17_API AP17WeaponBase : public AActor
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP17WeaponBase();

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	_NODISCARD
	UBoxComponent* GetBoxComponent() const { return WeaponCollisionBox; };

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UStaticMeshComponent> WeaponMesh = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UBoxComponent> WeaponCollisionBox = nullptr;
};
