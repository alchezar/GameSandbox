// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16Projectile.h"
#include "P16BlastBall.generated.h"

UCLASS()
class PROJECT16_API AP16BlastBall : public AP16Projectile
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16BlastBall();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaTime) override;
	virtual void Destroyed() override;

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
protected:
	virtual void OnSphereBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void CheckTarget() override;
	virtual void PlayOnHitEffects() const override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayOnHitCue() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Move")
	FVector Start = {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Move")
	FVector End = {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Move")
	float Distance = 1000.f;
};
