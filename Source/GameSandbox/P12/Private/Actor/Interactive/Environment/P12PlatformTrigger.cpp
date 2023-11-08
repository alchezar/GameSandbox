// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Interactive/Environment/P12PlatformTrigger.h"

#include "Components/BoxComponent.h"

AP12PlatformTrigger::AP12PlatformTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	
	TriggerBase = CreateDefaultSubobject<UStaticMeshComponent>("TriggerBaseStaticMeshComponent");
	TriggerBase->SetupAttachment(RootComponent);
	
	PlatformTrigger = CreateDefaultSubobject<UBoxComponent>("PlatformTriggerBoxComponent");
	PlatformTrigger->SetCollisionProfileName("PawnInteractionVolume");
	PlatformTrigger->SetupAttachment(RootComponent);
}

void AP12PlatformTrigger::BeginPlay()
{
	Super::BeginPlay();

	PlatformTrigger->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTriggerBeginOverlapHandle);
	PlatformTrigger->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnTriggerEndOverlapHandle);

	DynamicMaterial = TriggerBase->CreateAndSetMaterialInstanceDynamic(0);
	SaveInactiveAppearance(DynamicMaterial);
}

void AP12PlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP12PlatformTrigger::OnTriggerBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	APawn* OverlappedPawn = Cast<APawn>(OtherActor);
	if (!OverlappedPawn)
	{
		return;
	}
	OverlappedPawns.AddUnique(OverlappedPawn);
	if (!bActive && OverlappedPawns.Num() > 0)
	{
		SetIsActivated(true);
	}
}

void AP12PlatformTrigger::OnTriggerEndOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex) 
{
	APawn* OverlappedPawn = Cast<APawn>(OtherActor);
	if (!OverlappedPawn)
	{
		return;
	}
	OverlappedPawns.RemoveSingleSwap(OverlappedPawn);
	if (bActive && OverlappedPawns.IsEmpty())
	{
		SetIsActivated(false);
	}
}

void AP12PlatformTrigger::SetIsActivated(const bool bNewActive)
{
	bActive = bNewActive;
	OnTriggerActivated.Broadcast(bNewActive);

	DynamicMaterial->SetVectorParameterValue("SurfaceColor", bNewActive ? ActiveAppearance.Color : InactiveAppearance.Color);
	DynamicMaterial->SetVectorParameterValue("Emission", bNewActive ? ActiveAppearance.Emission : InactiveAppearance.Emission);
}

void AP12PlatformTrigger::SaveInactiveAppearance(const UMaterialInstanceDynamic* CurrentDynamicMaterial)
{
	CurrentDynamicMaterial->GetVectorParameterValue(TEXT("SurfaceColor"), InactiveAppearance.Color);
	CurrentDynamicMaterial->GetVectorParameterValue(TEXT("Emissive"), InactiveAppearance.Emission);
}

