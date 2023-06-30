// Copyright (C) 2023, IKinder

#include "P7/Public/Breakable/P7BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"

AP7BreakableActor::AP7BreakableActor()
{
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>("GeometryCollection");
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GeometryCollection->SetNotifyBreaks(true);
}

void AP7BreakableActor::BeginPlay()
{
	Super::BeginPlay();
	GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ThisClass::OnBreakHandle);
}

void AP7BreakableActor::GetHit(const FVector& ImpactPoint)
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SmashSound, ImpactPoint);
}

void AP7BreakableActor::OnBreakHandle(const FChaosBreakEvent& BreakEvent)
{
	SetLifeSpan(5.f);
}
