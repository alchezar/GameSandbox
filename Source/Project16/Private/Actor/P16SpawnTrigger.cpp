// Copyright © 2025, Ivan Kinder

#include "Actor/P16SpawnTrigger.h"

#include "Project16.h"
#include "Actor/P16SpawnPoint.h"
#include "Components/BoxComponent.h"
#include "Interface/P16PlayerInterface.h"

AP16SpawnTrigger::AP16SpawnTrigger()
{
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>("TriggerVolumeBoxComponent");
	TriggerVolume->SetupAttachment(GetRootComponent());
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	TriggerVolume->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AP16SpawnTrigger::BeginPlay()
{
	Super::BeginPlay();

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTriggerBeginOverlapCallback);
}

void AP16SpawnTrigger::LoadActor_Implementation()
{
	if (bReached)
	{
		Destroy();
	}
}

void AP16SpawnTrigger::OnTriggerBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EARLY_RETURN_IF(bReached || !OtherActor || !OtherActor->Implements<UP16PlayerInterface>())

	for (AP16SpawnPoint* Spawner : Spawners)
	{
		CONTINUE_IF(!Spawner)
		Spawner->Spawn();
	}
	bReached = true;
}
