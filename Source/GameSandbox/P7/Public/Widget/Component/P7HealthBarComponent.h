// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "P7HealthBarComponent.generated.h"

class UP7HealthBarWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP7HealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UP7HealthBarComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetHealthPercent(const float HealthPercent);
	void ReactOnDamage(const float HealthPercent, const bool bWidgetVisibility);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UP7HealthBarWidget* HealthBarWidget;
};
