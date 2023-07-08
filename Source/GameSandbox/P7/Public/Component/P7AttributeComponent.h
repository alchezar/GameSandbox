// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P7AttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FP7OnReceiveDamageDelegate, float /* HealthPercent */);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP7AttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UP7AttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FORCEINLINE float GetHealthPercent() const { return CurrentHealth / MaxHealth; };
	FORCEINLINE bool GetIsAlive() const { return CurrentHealth > 0.f; };
	void ReceiveDamage(const float Damage);
	void ResetHealth();

protected:
	virtual void BeginPlay() override;

public:
	FP7OnReceiveDamageDelegate OnReceiveDamage;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Health")
	float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Health")
	float CurrentHealth = 0.f;	
};
