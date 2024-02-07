// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "P13DamageDisplayComponent.generated.h"

class UP13DamageDisplayWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP13DamageDisplayComponent : public USceneComponent
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	UP13DamageDisplayComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* ------------------------------- This -------------------------------- */
public:
	void DisplayDamage(const float CurrentDamage, const float HealthAlpha);
	void DisplayShield(const float CurrentDamage, const float ShieldAlpha);
	UP13DamageDisplayWidget* CreateDamageWidget(FVector2D& Offset) const;

protected:
	void OnOwnerHealthChangedHandle(const float NewHealth, const float LastDamage, const float HealthAlpha);

	/* ------------------------------ Network ------------------------------ */
public:
	UFUNCTION(Server, Reliable)
	void Server_DisplayDamage(const float CurrentDamage, const float HealthAlpha);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DisplayDamage(const float CurrentDamage, const float HealthAlpha);
	UFUNCTION(Server, Reliable)
	void Server_DisplayShield(const float CurrentDamage, const float ShieldAlpha);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DisplayShield(const float CurrentDamage, const float ShieldAlpha);
	
	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Damage Display")
	TSubclassOf<UP13DamageDisplayWidget> DamageDisplayWidgetClass;
};
