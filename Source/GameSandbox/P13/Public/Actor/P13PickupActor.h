// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P13/Public/Library/P13Types.h"
#include "P13PickupActor.generated.h"

class USphereComponent;

UENUM(BlueprintType)
enum class EP13PickupType : uint8
{
	Ammo,
	Weapon
};

UCLASS()
class GAMESANDBOX_API AP13PickupActor : public AActor
{
	GENERATED_BODY()

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                Super                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	AP13PickupActor();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:
	virtual void Tick(const float DeltaTime) override;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                 This                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	void InitDrop(const FP13WeaponDrop* DropWeaponInfo);

protected:
	UFUNCTION()
	void OnCollisionBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void CreateComponents();
	void OnPickupSuccess();

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USphereComponent* Collision;

	UPROPERTY(EditAnywhere, Category = "C++ | Condition")
	EP13PickupType PickupType = EP13PickupType::Weapon;

	UPROPERTY(EditAnywhere, Category = "C++ | Ammo")
	FP13AmmoSlot AmmoSlot;

	UPROPERTY(EditAnywhere, Category = "C++ | Weapon", meta = (EditCondition = "PickupType == EP13PickupType::Weapon"))
	FP13WeaponSlot WeaponSlot;

	UPROPERTY(EditAnywhere, Category = "C++ | Drop")
	float ActivationDelay = 3.f;

private:
	FTimerHandle DropTimer;
	bool bDropped = false;
};
