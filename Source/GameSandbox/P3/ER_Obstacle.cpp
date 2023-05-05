// Copyright (C) 2023, IKinder

#include "ER_Obstacle.h"

#include "ER_Character.h"
#include "Components/BoxComponent.h"

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

	// StaticMesh->OnComponentHit.AddDynamic(this, &ThisClass::OnObstacleHit);
	KillTrigger->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnObstacleOverlap);
}

void AER_Obstacle::OnObstacleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (const auto Character = Cast<AER_Character>(OtherActor))
	{
		Character->Death();
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("%s"), *OtherActor->GetActorNameOrLabel()));
}

float AER_Obstacle::GetSpawnProbability() const
{
	return SpawnProbability;
}
