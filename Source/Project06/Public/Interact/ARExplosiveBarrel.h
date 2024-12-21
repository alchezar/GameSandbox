// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARExplosiveBarrel.generated.h"

class URadialForceComponent;

UCLASS()
class PROJECT06_API AARExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	AARExplosiveBarrel();
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBarrelHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Component")
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Component")
	URadialForceComponent* ForceComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Damage")
	float Damage = 40.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Damage")
	float Impulse = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Damage")
	float Radius = 750.f;
};
