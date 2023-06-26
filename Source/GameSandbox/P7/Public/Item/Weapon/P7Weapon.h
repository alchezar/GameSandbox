// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P7/Public/Item/P7Item.h"
#include "P7Weapon.generated.h"

UCLASS()
class GAMESANDBOX_API AP7Weapon : public AP7Item
{
	GENERATED_BODY()

public:
	AP7Weapon();
	virtual void Tick(float DeltaTime) override;

	void Equip(USceneComponent* InParent, FName SocketName);

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
