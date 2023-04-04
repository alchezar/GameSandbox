// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "PickUpBattery.generated.h"

UCLASS()
class GAMESANDBOX_API APickUpBattery : public APickUpBase
{
	GENERATED_BODY()

#pragma region Default

public:
	APickUpBattery();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

#pragma endregion // Default

#pragma region Charge

public:
	virtual void OnPickupCollect_Implementation() override;

	UFUNCTION(BlueprintPure, Category = "Kinder | Charge")
	float GetChargeAmount() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Charge", meta = (BlueprintProtected = "true"))
	float ChargeAmount = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Charge")
	float Lifetime = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Charge")
	UParticleSystem* Effect;

private:
	void DestroyItself();
	void UpdateTarget() const;
	void CreateEmitter();

	UPROPERTY()
	UParticleSystemComponent* Emitter;

#pragma endregion // Charge
};
