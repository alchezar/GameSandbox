// Copyright (C) 2023, IKinder

#include "STU_DevDamageActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ASTU_DevDamageActor::ASTU_DevDamageActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
}

void ASTU_DevDamageActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASTU_DevDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, Segments, SphereColor);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, bDoFullDamage);
}
