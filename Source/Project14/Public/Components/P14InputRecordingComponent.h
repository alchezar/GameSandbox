// Copyright © 2024, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utils/P14InputRecordingTypes.h"
#include "P14InputRecordingComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT14_API UP14InputRecordingComponent : public UActorComponent
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP14InputRecordingComponent();
	virtual void TickComponent(const float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/* ------------------------------- This -------------------------------- */
private:
	void SaveBindingsData();

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY()
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComp = nullptr;

private:
	FP14InputData InputData;
};
