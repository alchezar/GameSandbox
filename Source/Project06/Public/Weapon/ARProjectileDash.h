// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "ARProjectileBase.h"
#include "ARProjectileDash.generated.h"

UCLASS()
class PROJECT06_API AARProjectileDash : public AARProjectileBase
{
	GENERATED_BODY()

public:
	AARProjectileDash();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void Explode() override;
	void TeleportInstigator();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Teleport")
	float DetonateDelay = 0.2f;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Teleport")
	float TeleportDelay = 0.2f;
};
