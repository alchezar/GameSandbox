// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "ARPowerupBase.h"
#include "ARPowerupHealthPotion.generated.h"

class UStaticMeshComponent;

UCLASS()
class PROJECT06_API AARPowerupHealthPotion : public AARPowerupBase
{
	GENERATED_BODY()

public:
	AARPowerupHealthPotion();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Component")
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Credit")
	int32 CreditsCost = 5;
};
