// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "ARProjectileBase.h"
#include "ARProjectileHole.generated.h"

class URadialForceComponent;

UCLASS()
class GAMESANDBOX_API AARProjectileHole : public AARProjectileBase
{
	GENERATED_BODY()

public:
	AARProjectileHole();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Force")
	URadialForceComponent* ForceComp;
	
};
