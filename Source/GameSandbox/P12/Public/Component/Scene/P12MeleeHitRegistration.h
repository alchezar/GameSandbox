// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "P12MeleeHitRegistration.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FP12OnMeleeRegisteredSignature, const FHitResult&, const FVector&)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP12MeleeHitRegistration : public USphereComponent
{
	GENERATED_BODY()

public:
	UP12MeleeHitRegistration();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FORCEINLINE void SetIsHitRegistration(const bool bNewHitRegistration) { bHitRegistration = bNewHitRegistration; };
	void ProcessHitRegistration();

protected:
	virtual void BeginPlay() override;

public:
	FP12OnMeleeRegisteredSignature OnMeleeRegistered;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	bool bHitRegistration = false;

private:
	FVector PreviousLocation = FVector::ZeroVector;
};
