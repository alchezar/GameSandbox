// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARAttributesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAROnHealthChangedSignature, AActor*, InstigatorActor, UARAttributesComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAROnDeadSignature, AActor*, DeadActor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UARAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UARAttributesComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool TryChangeHealth(AActor* InstigatorActor, const float Delta);
	bool GetIsAlive() const;
	bool GetIsHealthMax() const;
	float GetHealthMax() const;
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	static UARAttributesComponent* GetAttributes(AActor* FromActor);
	UFUNCTION(BlueprintCallable)
	static bool GetIsActorAlive(AActor* Actor);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "C++ | Delegate")
	FAROnHealthChangedSignature AROnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "C++ | Delegate")
	FAROnDeadSignature AROnDead;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Health")
	float HealthMax = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++ | Health")
	float Health;

private:
};
