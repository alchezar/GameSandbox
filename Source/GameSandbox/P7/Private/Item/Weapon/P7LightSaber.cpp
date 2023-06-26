// Copyright (C) 2023, IKinder

#include "P7/Public/Item/Weapon/P7LightSaber.h"

AP7LightSaber::AP7LightSaber()
{
	PrimaryActorTick.bCanEverTick = true;

	Beam = CreateDefaultSubobject<UStaticMeshComponent>("BeamMesh");
	Beam->SetupAttachment(ItemMesh);
	Beam->SetCollisionProfileName("NoCollision");
}

void AP7LightSaber::BeginPlay()
{
	Super::BeginPlay();
}

void AP7LightSaber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP7LightSaber::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AP7LightSaber::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}