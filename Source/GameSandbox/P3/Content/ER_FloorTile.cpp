// Copyright (C) 2023, IKinder

#include "ER_FloorTile.h"
#include "ER_CoinItem.h"
#include "ER_Obstacle.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ER_Character.h"
#include "Player/ER_GameModeBase.h"

AER_FloorTile::AER_FloorTile()
{
	PrimaryActorTick.bCanEverTick = true;

	RunGameMode = Cast<AER_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	SetupComponents();
}

void AER_FloorTile::BeginPlay()
{
	Super::BeginPlay();
	check(RunGameMode);

	FloorTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTriggerBeginOverlap);
}

void AER_FloorTile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AER_FloorTile::SetupComponents()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>("FloorMesh");
	FloorMesh->SetupAttachment(GetRootComponent());

	AttachPoint = CreateDefaultSubobject<UArrowComponent>("AttachPoint");
	AttachPoint->SetupAttachment(GetRootComponent());

	CenterLane = CreateDefaultSubobject<UArrowComponent>("CenterLane");
	CenterLane->SetupAttachment(GetRootComponent());

	LeftLane = CreateDefaultSubobject<UArrowComponent>("LeftLane");
	LeftLane->SetupAttachment(GetRootComponent());

	RightLane = CreateDefaultSubobject<UArrowComponent>("RightLane");
	RightLane->SetupAttachment(GetRootComponent());

	FloorTriggerBox = CreateDefaultSubobject<UBoxComponent>("FloorTriggerBox");
	FloorTriggerBox->SetupAttachment(GetRootComponent());
	FloorTriggerBox->SetBoxExtent(FVector(32.f, 500.f, 200.f));
	FloorTriggerBox->SetCollisionProfileName("OverlapOnlyPawn");
}

void AER_FloorTile::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !Cast<AER_Character>(OtherActor)) return;

	RunGameMode->AddFloorTile(true);

	FTimerHandle DestroyTimer;
	GetWorld()->GetTimerManager().SetTimer(OUT DestroyTimer, this, &ThisClass::DestroyFloorTile, DestroyDelay);
}

void AER_FloorTile::DestroyFloorTile()
{
	for (const auto Coin : Coins)
	{
		Coin->Destroy();
	}
	for (const auto Obstacle : Obstacles)
	{
		Obstacle->Destroy();
	}
	RunGameMode->RemoveFloorTile(this);
	Destroy();
}

const FTransform& AER_FloorTile::GetAttachPoint() const
{
	return AttachPoint->GetComponentTransform();
}

TArray<FVector> AER_FloorTile::GetLaneMidLocations() const
{
	TArray<FVector> LaneShiftValues;

	LaneShiftValues.Add(LeftLane->GetComponentLocation());
	LaneShiftValues.Add(CenterLane->GetComponentLocation());
	LaneShiftValues.Add(RightLane->GetComponentLocation());

	return LaneShiftValues;
}

void AER_FloorTile::SpawnItems()
{
	const TArray<FVector> LaneMidLocations = GetLaneMidLocations();
	if (LaneMidLocations.IsEmpty()) return;

	int32 LargeObstacles = 0;

	for (int i = 0; i < LaneMidLocations.Num(); ++i)
	{
		// Don`t spawn coins above large obstacle
		bool bLarge = false;
		SpawnObstacle(LaneMidLocations[i],OUT LargeObstacles,OUT bLarge);
		SpawnCoins(LaneMidLocations[i], bLarge);
		SpawnSideCoins(LaneMidLocations[i]);
	}
}

void AER_FloorTile::SpawnObstacle(const FVector& LaneMidLocation, int& LargeNum, bool& bLarge)
{
	const int32 ObstacleRandomIndex = FMath::RandRange(0, ObstacleClasses.Num() - 1);

	// Prevent spawning all big obstacles in one row
	const bool bCandidate = LargeObstacleIndexes.Contains(ObstacleRandomIndex);
	if (bCandidate && ++LargeNum > 2) return;

	// Choose obstacle and spawn it
	const TSubclassOf<AER_Obstacle> ObstacleClass = ObstacleClasses[ObstacleRandomIndex];
	if (!ObstacleClass) return;

	const FVector SpawnLocation     = LaneMidLocation;
	const FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AER_Obstacle* Obstacle = GetWorld()->SpawnActor<AER_Obstacle>(ObstacleClass, SpawnTransform, SpawnParameters);
	if (!Obstacle) return;

	if (FMath::FRand() <= Obstacle->GetSpawnProbability())
	{
		Obstacles.Add(Obstacle);
		bLarge = bCandidate;
		return;
	}
	Obstacle->Destroy();
}

void AER_FloorTile::SpawnCoins(const FVector& LaneMidLocation, const bool bLarge)
{
	if (bLarge) return;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (AER_CoinItem* Coin = GetWorld()->SpawnActor<AER_CoinItem>(CoinClass, LaneMidLocation, FRotator::ZeroRotator, SpawnParameters))
	{
		CoinsSpawnProbability = Coin->GetSpawnProbability();
		(FMath::FRand() <= CoinsSpawnProbability) ? Coins.Add(Coin) : Coin->Destroy();
	}
}

void AER_FloorTile::SpawnSideCoins(const FVector& LaneMidLocation)
{
	FVector SpawnLocation = LaneMidLocation;

	// Additional coins before and after center
	const double Interval = FloorMesh->Bounds.GetBox().GetSize().X * 0.33;
	for (int i = 1; i <= 2; ++i)
	{
		const float Side = (i == 1) ? 1.f : -1.f;
		if (FMath::FRand() <= CoinsSpawnProbability)
		{
			// If move back 1 interval, it will be same location as LaneMidLocation, so multiply interval by 2(i)
			SpawnLocation.X += Side * Interval * i;
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AER_CoinItem* SideCoin = GetWorld()->SpawnActor<AER_CoinItem>(CoinClass, SpawnLocation, FRotator::ZeroRotator, SpawnParameters);
			Coins.Add(SideCoin);
		}
	}
}
