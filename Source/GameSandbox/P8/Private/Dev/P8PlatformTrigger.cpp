// Copyright (C) 2023, IKinder

#include "P8/Public/Dev/P8PlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "P8/Public/Dev/P8MovablePlatform.h"

AP8PlatformTrigger::AP8PlatformTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	PressureVolume = CreateDefaultSubobject<UBoxComponent>("PressureVolume");
	PressureVolume->SetupAttachment(RootComponent);
	PressurePad = CreateDefaultSubobject<UStaticMeshComponent>("PressurePad");
	PressurePad->SetupAttachment(RootComponent);
}

void AP8PlatformTrigger::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	check(PressureVolume);
	PressureVolume->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnVolumeBeginOverlapHandle);
	PressureVolume->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnVolumeEndOverlapHandle);
}

void AP8PlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void AP8PlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP8PlatformTrigger::OnVolumeBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MovePlatforms(true);
}

void AP8PlatformTrigger::OnVolumeEndOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	MovePlatforms(false);
}

void AP8PlatformTrigger::MovePlatforms(const bool bMove)
{
	if (MovablePlatforms.IsEmpty()) return;
	for (AP8MovablePlatform* MovablePlatform : MovablePlatforms)
	{
		if (!MovablePlatform) return;
		MovablePlatform->SetTriggerActivation(bMove);
	}	
}
