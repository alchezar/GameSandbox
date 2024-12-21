// Copyright (C) 2023, IKinder

#include "Game/TG_TerrainTile.h"

#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Game/TG_ActorPool.h"
#include "Game/TG_GameMode.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Player/TG_FirstPersonCharacter.h"

ATG_TerrainTile::ATG_TerrainTile()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	BackWall = CreateDefaultSubobject<UStaticMeshComponent>("BackWall");
	BackWall->SetupAttachment(GetRootComponent());

	FrontWall = CreateDefaultSubobject<UStaticMeshComponent>("FrontWall");
	FrontWall->SetupAttachment(GetRootComponent());

	NextTileLocation = CreateDefaultSubobject<UArrowComponent>("NextTileLocation");
	NextTileLocation->SetupAttachment(GetRootComponent());

	LockWall = CreateDefaultSubobject<UStaticMeshComponent>("LockWall");
	LockWall->SetupAttachment(GetRootComponent());
}

void ATG_TerrainTile::BeginPlay()
{
	Super::BeginPlay();
	
	LockWall->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnLockOverlapped);
	GameMode = Cast<ATG_GameMode>(GetWorld()->GetAuthGameMode());

	PlaceCovers();
	if (!bAlwaysUnlock)
	{
		Lock();
		PlaceTroopers(true);
	}
}

void ATG_TerrainTile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ActorPool && NavMeshVolume)
	{
		ActorPool->Return(NavMeshVolume);
	}

	Super::EndPlay(EndPlayReason);
}

void ATG_TerrainTile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ATG_TerrainTile::GetAttachPoint()
{
	return NextTileLocation->GetComponentLocation();
}

bool ATG_TerrainTile::GetIsAlwaysUnlocked() const
{
	return bAlwaysUnlock;
}

void ATG_TerrainTile::SetNextTile(ATG_TerrainTile* NewNextTile)
{
	NextTile = NewNextTile;
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void ATG_TerrainTile::OnLockOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ATG_FirstPersonCharacter::StaticClass()))
	{
		Lock();
		if (ATG_FirstPersonCharacter* Character = Cast<ATG_FirstPersonCharacter>(OtherActor))
		{
			Character->SetHealth(Character->GetMaxHealth());
		}
		
		if (GameMode)
		{
			GameMode->UpdateActiveTiles();
		}
	}
}

void ATG_TerrainTile::Lock()
{
	LockWall->SetCollisionProfileName("BlockAll");
	LockWall->SetMaterial(0, LockedAppearance);

	if (NextTile)
	{
		NextTile->Unlock();
	}
}

void ATG_TerrainTile::Unlock()
{
	LockWall->SetCollisionProfileName("OverlapOnlyPawn");
	LockWall->SetMaterial(0, UnlockedAppearance);
}

void ATG_TerrainTile::PlaceCovers()
{
	if (ActorsToSpawn.IsEmpty()) return;

	constexpr int MaxAttempts = 10;
	int           Attempts = NULL;

	// const TInterval Diapason = TInterval(FVector(0.f, -1500.f, 0.f), FVector(3000.f, 1500.f, 0.f));
	const int32 CoversNum = FMath::RandRange(SpawnedActorsNum.Min, SpawnedActorsNum.Max);
	for (int i = 0; i < CoversNum || Attempts >= MaxAttempts; ++i)
	{
		/* Get random item from actors array */
		const int32 InRangeIndex = FMath::RandRange(0, ActorsToSpawn.Num() - 1);
		/* Get random location and snap it to grid 50*50 */
		FVector Location = FMath::RandPointInBox(FBox(MinExtent, MaxExtent));
		Location.X = static_cast<int32>(Location.X / 50) * 50;
		Location.Y = static_cast<int32>(Location.Y / 50) * 50;
		/* Get random rotation with 45⁰ steps */
		const FRotator Rotation = FRotator(0.f, FMath::RandRange(0, 7) * 45.f, 0.f);
		/* If there are any other spawned actors near this location - try another location */
		FVector     GlobalLocation = ActorToWorld().TransformPosition(Location);
		if (!FindEmptyLocation(GlobalLocation, ActorsToSpawn[InRangeIndex].Radius))
		{
			--i;
			++Attempts;
			continue;
		}
		/* Spawn actor and set it transforms */
		AActor* SpawnedActor = GetWorld()->SpawnActor(ActorsToSpawn[InRangeIndex].Class, &Location);
		SpawnedActor->SetActorRotation(Rotation);
		SpawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

		CoverActors.Add(SpawnedActor);
		Attempts = NULL;
	}
}

void ATG_TerrainTile::PlaceTroopers(const bool bEnemy)
{
	if (!PawnActor.Class) return;

	for (int i = 0; i < FMath::RandRange(PawnNum.Min, PawnNum.Max); ++i)
	{
		FVector ClampedMinExtent = MinExtent;
		FVector ClampedMaxExtent = MaxExtent;
		ClampedMinExtent.X = bEnemy ? 1500.f : 0.f;
		ClampedMaxExtent.X = bEnemy ? 3000.f : 1500.f;

		FVector        Location = FMath::RandPointInBox(FBox(ClampedMinExtent, ClampedMaxExtent));
		const FRotator Rotation = FRotator(0.f, bEnemy ? 180.f : 0.f, 0.f);

		ATG_BaseCharacter* Trooper = GetWorld()->SpawnActor<ATG_BaseCharacter>(PawnActor.Class);
		if (!Trooper) return;

		Trooper->SetActorLocation(ActorToWorld().TransformPosition(Location));
		Trooper->SetActorRotation(Rotation);
		Trooper->SpawnDefaultController();
		Trooper->AutoPossessAI = EAutoPossessAI::PlacedInWorld;
		Trooper->Tags.Add(bEnemy ? "Bot" : "Friend");

		bEnemy ? Enemies.Add(Trooper) : Friends.Add(Trooper);
	}
}

bool ATG_TerrainTile::FindEmptyLocation(const FVector& Location, const float Radius)
{
	FHitResult SweepHit;
	const bool bHit = GetWorld()->SweepSingleByChannel(SweepHit, Location, Location, FQuat::Identity, ECC_GameTraceChannel4, FCollisionShape::MakeSphere(Radius));
	return !bHit;
}

void ATG_TerrainTile::DestroyTile()
{
	for (const auto Cover : CoverActors)
	{
		Cover->Destroy();
	}
	Destroy();
}

void ATG_TerrainTile::SetActorPool(UTG_ActorPool* TheActorPool)
{
	ActorPool = TheActorPool;

	NavMeshVolume = ActorPool->Checkout();
	if (!NavMeshVolume) return;

	NavMeshVolume->SetActorLocation(GetActorLocation() + FVector(MaxExtent.X / 2, 0.f, 0.f));
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return;

	NavSys->Build();
}
