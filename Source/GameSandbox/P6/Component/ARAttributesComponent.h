// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARAttributesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAROnHealthChangedSignature, AActor*, InstigatorActor, UARAttributesComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UARAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UARAttributesComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool TryChangeHealth(const float Delta);
	bool GetIsAlive() const;


protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "C++ | Delegate")
	FAROnHealthChangedSignature AROnHealthChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Health")
	float HealthMax = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Health")
	float Health;

private:
};
