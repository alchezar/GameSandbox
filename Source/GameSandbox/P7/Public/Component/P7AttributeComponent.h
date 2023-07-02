// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P7AttributeComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP7AttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UP7AttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ReceiveDamage(const float Damage);
	FORCEINLINE float GetHealthPercent() const { return Health / MaxHealth; };
	FORCEINLINE bool GetIsAlive() const { return Health > 0.f; };

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Health")
	float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Health")
	float Health = 0.f;	
};
