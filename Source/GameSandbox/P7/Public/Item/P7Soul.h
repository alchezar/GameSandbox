// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P7Item.h"
#include "P7Soul.generated.h"

class UNiagaraSystem;

UCLASS()
class GAMESANDBOX_API AP7Soul : public AP7Item
{
	GENERATED_BODY()

public:
	AP7Soul();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Gold")
	int32 Souls = 1;
};
