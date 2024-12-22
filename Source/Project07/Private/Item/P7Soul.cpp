// Copyright (C) 2023, IKinder

#include "Item/P7Soul.h"

#include "Player/P7Character.h"

AP7Soul::AP7Soul()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP7Soul::BeginPlay()
{
	Super::BeginPlay();
}

void AP7Soul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP7Soul::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IP7PickupInterface* PickupInterface = Cast<IP7PickupInterface>(OtherActor))
	{
		PickupInterface->AddSouls(Souls);
		SpawnPickupFX();
		Destroy();
	}
}

