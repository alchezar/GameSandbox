// Copyright (C) 2023, IKinder

#include "P7/Public/Breakable/P7BreakableActor.h"
#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "P7/Public/Item/Treasure/P7Treasure.h"

AP7BreakableActor::AP7BreakableActor()
{
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>("GeometryCollection");
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GeometryCollection->SetNotifyBreaks(true);
	SetRootComponent(GeometryCollection);

	CapsulePawnBlocker = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsulePawnBlocker->SetupAttachment(RootComponent);
	CapsulePawnBlocker->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsulePawnBlocker->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CapsulePawnBlocker->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	CapsulePawnBlocker->SetCapsuleRadius(50.f);
	CapsulePawnBlocker->SetCapsuleHalfHeight(100.f);
}

void AP7BreakableActor::BeginPlay()
{
	Super::BeginPlay();
	GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ThisClass::OnBreakHandle);
}

void AP7BreakableActor::GetHit(const FVector& HitterLocation)
{
	if (!GetWorld() || !TreasureClass) return;
	
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SmashSound, HitterLocation);
	const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 100.f);
	const FRotator SpawnRotation = GetActorRotation() + FRotator(90.f, 0.f, 0.f);
	GetWorld()->SpawnActor<AP7Treasure>(TreasureClass, SpawnLocation, SpawnRotation);
}

void AP7BreakableActor::OnBreakHandle(const FChaosBreakEvent& BreakEvent)
{
	CapsulePawnBlocker->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	SetLifeSpan(5.f);
}
