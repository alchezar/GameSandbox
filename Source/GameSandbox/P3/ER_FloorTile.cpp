// Copyright (C) 2023, IKinder

#include "ER_FloorTile.h"
#include "ER_Character.h"
#include "ER_GameModeBase.h"
#include "ER_Obstacle.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

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
	for (const auto Obstacle : Obstacles)
	{
		Obstacle->Destroy();
	}	
	Destroy();
}

const FTransform& AER_FloorTile::GetAttachPoint() const
{
	return AttachPoint->GetComponentTransform();
}

TArray<float> AER_FloorTile::GetLaneShiftValues() const
{
	TArray<float> LaneShiftValues;

	LaneShiftValues.Add(LeftLane->GetComponentLocation().Y);
	LaneShiftValues.Add(CenterLane->GetComponentLocation().Y);
	LaneShiftValues.Add(RightLane->GetComponentLocation().Y);

	return LaneShiftValues;
}

void AER_FloorTile::SpawnObstacles()
{
	const TArray<float> LaneShiftValues = GetLaneShiftValues();
	if (LaneShiftValues.IsEmpty()) return;

	for (int i = 0; i < LaneShiftValues.Num(); ++i)
	{
		SpawnOneObstacle(LaneShiftValues[i]);
	}
}

void AER_FloorTile::SpawnOneObstacle(const float LaneLocation)
{
	const int32 RandomProbability = FMath::RandRange(0, ObstacleClasses.Num() - 1);

	const TSubclassOf<AER_Obstacle> ObstacleClass = ObstacleClasses[RandomProbability];
	if (!ObstacleClass) return;

	FVector SpawnLocation           = CenterLane->GetComponentLocation();
	SpawnLocation.Y                 = LaneLocation;
	const FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation);

	AER_Obstacle* Obstacle = GetWorld()->SpawnActorDeferred<AER_Obstacle>(ObstacleClass, SpawnTransform);
	if (!Obstacle) return;
	if (FMath::FRand() > Obstacle->GetSpawnProbability())
	{
		Obstacle->Destroy();
		return;
	}
	Obstacle->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Obstacle->FinishSpawning(SpawnTransform);
	Obstacles.Add(Obstacle);
}
