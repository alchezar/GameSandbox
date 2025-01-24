// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P12LedgeDetectionComponent.generated.h"

struct FP12LedgeDescription;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT12_API UP12LedgeDetectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UP12LedgeDetectionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool DetectLedge(OUT FP12LedgeDescription& LedgeDescription);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Detection", meta = (ClampMin = 0.f, UIMin = 0.f))
	float MinLedgeHeight = 40.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Detection", meta = (ClampMin = 0.f, UIMin = 0.f))
	float MaxLedgeHeight = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Detection", meta = (ClampMin = 0.f, UIMin = 0.f))
	float ForwardCheckDistance = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Detection", meta = (ClampMin = 0.f, UIMin = 0.f))
	float DownwardCheckDistance = 100.f;

private:
	TWeakObjectPtr<ACharacter> CachedOwnerCharacter;
};
