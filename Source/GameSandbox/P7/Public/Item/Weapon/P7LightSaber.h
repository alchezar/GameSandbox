// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P7/Public/Item/Weapon/P7Weapon.h"
#include "P7LightSaber.generated.h"

UCLASS()
class GAMESANDBOX_API AP7LightSaber : public AP7Weapon
{
	GENERATED_BODY()

public:
	AP7LightSaber();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Component")
	UStaticMeshComponent* Beam; 
};
