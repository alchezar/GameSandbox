// Copyright (C) 2023, IKinder

#include "GameSandbox/P1/Spawners/BatterySpawnerBase.h"
#include "Components/BoxComponent.h"
#include "GameSandbox/P1/PickUp/PickupBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BillboardComponent.h"

#define OUT 

ABatterySpawnerBase::ABatterySpawnerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = SceneRoot;

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Volume"));
	SpawnVolume->SetLineThickness(3.f);
	SpawnVolume->SetupAttachment(SceneRoot);

#pragma region Sprite
	SpriteComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	if (!SpriteComponent) return;
	static ConstructorHelpers::FObjectFinder<UTexture2D> TriggerTextureFinder(TEXT("/Engine/EditorResources/S_Actor"));
	SpriteComponent->Sprite              = TriggerTextureFinder.Object;
	SpriteComponent->bHiddenInGame       = true;
	SpriteComponent->bIsScreenSizeScaled = true;
	SpriteComponent->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	SpriteComponent->SetupAttachment(SceneRoot);
#pragma endregion
}

void ABatterySpawnerBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(OUT Timer, this, &ABatterySpawnerBase::SpawnActor, 1.f, true);
}

void ABatterySpawnerBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABatterySpawnerBase::SetSpawnerActive(const bool bActive)
{
	if (bActive)
	{
		SpawnActor();
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(OUT Timer);
	}
}

FVector ABatterySpawnerBase::GetRandomSpawnPoint() const
{
	const FVector SpawnOrigin = SpawnVolume->Bounds.Origin;
	const FVector SpawnLimit  = SpawnVolume->Bounds.BoxExtent;
	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnLimit);
}

void ABatterySpawnerBase::SpawnActor()
{
	if (!GetWorld()) return;
	RandomSpawnDelay = FMath::FRandRange(MinSpawnDelay, MaxSpawnDelay);
	GetWorld()->GetTimerManager().SetTimer(OUT Timer, this, &ABatterySpawnerBase::SpawnActor, RandomSpawnDelay, true);

	if (!ActorToSpawn) return;
	FRotator RandomRotation;
	RandomRotation.Yaw   = FMath::RandRange(0, 360);
	RandomRotation.Pitch = FMath::RandRange(0, 360);
	RandomRotation.Roll  = FMath::RandRange(0, 360);

	FActorSpawnParameters Params;
	Params.Owner                          = this;
	Params.Instigator                     = GetInstigator();
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	const APickUpBase* SpawnedActor = GetWorld()->SpawnActor<APickUpBase>(ActorToSpawn, GetRandomSpawnPoint(), RandomRotation, Params);
	if (!SpawnedActor) return;

}
