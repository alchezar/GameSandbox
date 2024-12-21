// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ARGameplayInterface.h"
#include "ARPowerupBase.generated.h"

class USphereComponent;

UCLASS()
class PROJECT06_API AARPowerupBase : public AActor, public  IARGameplayInterface
{
	GENERATED_BODY()

public:
	AARPowerupBase();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	/* Multiplayer */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void ShowPowerup();
	void CooldownPowerup();

	UFUNCTION()
	void OnRep_Active();

private:
	void SetPowerupState(bool bNewState);
		
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Component")
	USphereComponent* SphereComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Powerup")
	float RespawnTime = 10.f;

	UPROPERTY(ReplicatedUsing = OnRep_Active)
	bool bActive = false;

private:
	FTimerHandle RespawnTimer;
	
};
