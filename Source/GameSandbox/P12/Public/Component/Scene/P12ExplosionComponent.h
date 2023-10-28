// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "P12ExplosionComponent.generated.h"

USTRUCT(BlueprintType)
struct FP12Diapason
{
	GENERATED_BODY()

	FP12Diapason()
		{}
	FP12Diapason(const float NewMin, const float NewMax)
		: Min(NewMin), Max(NewMax) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Min = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max = 1.f;
	
};

DECLARE_MULTICAST_DELEGATE(FP12OnExplosionSignature)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP12ExplosionComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	void Explode(AController* Instigator);
	
public:
	FP12OnExplosionSignature OnExplosion;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Damage")
	FP12Diapason Damage = {30.f, 75.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Damage")
	FP12Diapason Radius = {300.f, 500.f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Damage")
	float DamageFallow = 1.f;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Damage")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | Effect")
	UParticleSystem* ExplosionEffect = nullptr;

};
