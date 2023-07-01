// Copyright (C) 2023, IKinder

#include "P7/Public/Item/Treasure/P7Treasure.h"
#include "Kismet/GameplayStatics.h"
#include "P7/Public/Player/P7Character.h"

AP7Treasure::AP7Treasure()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP7Treasure::BeginPlay()
{
	Super::BeginPlay();
}

void AP7Treasure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP7Treasure::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AP7Character* Character = Cast<AP7Character>(OtherActor))
	{
		// Character->AddCoin();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), TreasureSound, GetActorLocation());
		Destroy();
	}
}