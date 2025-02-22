// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARAttributesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAROnAttributeChangedSignature, AActor*, InstigatorActor, UARAttributesComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT06_API UARAttributesComponent : public UActorComponent
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

	/* Multiplayer */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	/* Multiplayer */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);
	
public:
	UPROPERTY(BlueprintAssignable, Category = "C++ | Delegate")
	FAROnAttributeChangedSignature AROnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "C++ | Delegate")
	FAROnAttributeChangedSignature AROnRageChanged;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "C++ | Health")
	float HealthMax = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "C++ | Health")
	float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "C++ | Rage")
	float RageMax = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "C++ | Rage")
	float Rage;	
};
