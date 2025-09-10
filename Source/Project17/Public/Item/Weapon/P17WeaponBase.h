// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P17WeaponBase.generated.h"

class UBoxComponent;

DECLARE_DELEGATE_OneParam(FOnTargetHit, AActor*);

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

	void ToggleCollision(const bool bEnable) const;

protected:
	UFUNCTION()
	virtual void OnCollisionBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnCollisionEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	FOnTargetHit OnWeaponHitTarget;
	FOnTargetHit OnWeaponPulledFromTarget;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UStaticMeshComponent> WeaponMesh = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "C++ | Component")
	TObjectPtr<UBoxComponent> WeaponCollisionBox = nullptr;
};
