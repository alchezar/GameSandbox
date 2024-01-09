// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P13PickupBase.generated.h"

class UNiagaraSystem;
class IP13PickupInterface;
class USphereComponent;

UCLASS(Abstract, NotBlueprintable)
class GAMESANDBOX_API AP13PickupBase : public AActor
{
	GENERATED_BODY()

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                Super                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	AP13PickupBase();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                 This                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	UFUNCTION()
	virtual void OnCollisionBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnPickupSuccess();
	
private:
	void CreateComponents();

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
	
	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	UNiagaraSystem* WaitEffect;
	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	UNiagaraSystem* PickupEffect;	
	UPROPERTY(EditAnywhere, Category = "C++ | Effect")
	FLinearColor EffectColor = FColor::White;
};
