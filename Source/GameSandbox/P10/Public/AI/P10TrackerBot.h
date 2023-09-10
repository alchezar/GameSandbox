// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "P10TrackerBot.generated.h"

class USphereComponent;
class UP10HealthComponent;
class UFloatingPawnMovement;

USTRUCT()
struct FP10TrackerBotSound
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	USoundBase* Rolling;
	UPROPERTY(EditAnywhere)
	USoundBase* TargetReached;
	UPROPERTY(EditAnywhere)
	USoundBase* SelfDestruct;
	UPROPERTY(EditAnywhere)
	USoundBase* Explode;
};

UENUM(BlueprintType)
enum class EP10TrackerBotState : uint8
{
	Alive,
	Reached,
	Countdown,
	Dead
};

UCLASS()
class GAMESANDBOX_API AP10TrackerBot : public APawn
{
	GENERATED_BODY()

public:
	AP10TrackerBot();
	virtual void Tick(float DeltaTime) override;
	void ForceSuicide();

protected:
	virtual void BeginPlay() override;
	void FindNextPathPoint(AActor* Goal);
	void FindTargetPawn();
	void MoveToNextPoint();
	void OnTargetReached();
	void OnHealthChangedHandle(UP10HealthComponent* Component, float Health, float Delta, const UDamageType* DamageType, AController* InstignatedBy, AActor* DamageCauser);
	void PulseOnHitHandle(UMaterialInstanceDynamic* PulseMaterial);
	UFUNCTION()
	void OnSphereBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(Server, Reliable)
	void Server_Suicide();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Suicide();
	void Suicide();

private:
	void CreateRollingAudioComponent();
	void PlayRollingSound() const;
	void FindDefaultReferences();

protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UP10HealthComponent* HealthComp;
	
	UPROPERTY(EditAnywhere, Category = "C++ | Move")
	float Force = 500.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Move")
	float MaxMissCompensation = 5.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Move")
	bool bVelocityChange = true;
	UPROPERTY(EditAnywhere, Category = "C++ | Move")
	float AcceptableDistance = 100.f;

	UPROPERTY(EditAnywhere, Category = "C++ | Damage")
	float SelfDamageInterval = 0.5f;
	UPROPERTY(EditAnywhere, Category = "C++ | Damage")
	float DamageRadius = 200.f;	
	UPROPERTY(EditAnywhere, Category = "C++ | Damage")
	float Damage = 40.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Damage", meta = (Units = "s"))
	float PulseTime = 0.1f;

	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	UParticleSystem* Explosion;
	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	FP10TrackerBotSound Sound;
	
private:
	UPROPERTY(Replicated)
	FVector NextPathPoint;
	FVector LastFrameLocation;
	UPROPERTY()
	APawn* TargetPawn;
	FTimerHandle PulseTimer;
	FTimerHandle KamikazeTimer;
	FTimerHandle RefreshPathTimer;
	float Alpha = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UAudioComponent* AudioComponent;
	EP10TrackerBotState BotState = EP10TrackerBotState::Alive;
};
