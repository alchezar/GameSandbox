// Copyright (C) 2023, IKinder

#include "ER_Obstacle.h"
#include "Components/BoxComponent.h"
#include "Player/ER_Character.h"

AER_Obstacle::AER_Obstacle()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(SceneComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(GetRootComponent());

	KillTrigger = CreateDefaultSubobject<UBoxComponent>("KillTrigger");
	KillTrigger->SetupAttachment(GetRootComponent());
	KillTrigger->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AER_Obstacle::BeginPlay()
{
	Super::BeginPlay();

	KillTrigger->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnObstacleOverlap);
}

void AER_Obstacle::OnObstacleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto Character = Cast<AER_Character>(OtherActor);
	if (Character && !Character->IsDead())
	{
		Character->Death();
	}
}
