// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P6/Interface/ARGameplayInterface.h"
#include "ARBonfire.generated.h"

class UParticleSystemComponent;

UCLASS()
class GAMESANDBOX_API AARBonfire : public AActor, public IARGameplayInterface
{
	GENERATED_BODY()

public:
	AARBonfire();

	/* Interface */
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual void OnActorLoaded_Implementation() override;

protected:
	virtual void BeginPlay() override;

private:
	void UpdateFlame();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Component")
	UStaticMeshComponent* Firewood;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Component")
	UParticleSystemComponent* FireEffect;

private:
	UPROPERTY(SaveGame)
	bool bActiveFire = false;
};
