// Copyright © 2024, IKinder

#include "Arkanoid/P13Blocks.h"

#include "Arkanoid/P13Ball.h"

AP13Blocks::AP13Blocks()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	SetRootComponent(SceneRoot);
}

void AP13Blocks::BeginPlay()
{
	Super::BeginPlay();
}

void AP13Blocks::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP13Blocks::OnConstruction(const FTransform& Transform)
{
	PlaceBlocks();
}

void AP13Blocks::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	OnBallHitHandle(MyComp, Other);
}

void AP13Blocks::PlaceBlocks()
{
	/* Clear previously created blocks to prevent accumulation of created components. */
	if (!SpawnedBlocks.IsEmpty())
	{
		for (UStaticMeshComponent* Block : SpawnedBlocks)
		{
			if (!Block)
			{
				continue;
			}
			Block->DestroyComponent();
		}
		SpawnedBlocks.Empty();
	}

	/* Spawn updated block components. */
	for (int Row = 0; Row < NumX; ++Row)
	{
		for (int Line = 0; Line < NumY; ++Line)
		{
			UStaticMeshComponent* BlockComponent = NewObject<UStaticMeshComponent>(this);
			if (!BlockComponent)
			{
				return;
			}

			SpawnedBlocks.Add(BlockComponent);
			BlockComponent->SetupAttachment(RootComponent);
			BlockComponent->SetStaticMesh(BlockMesh);
			BlockComponent->SetMaterial(0, BlockMaterial);
			BlockComponent->RegisterComponent();

			const FVector BoxExtent = BlockComponent->Bounds.GetBox().GetSize();
			const FVector BlockLocation = FVector(Line * BoxExtent.X, Row * BoxExtent.Y, BoxExtent.Z / 2.f);
			const FVector BlockScale = BoxExtent / Padding;

			BlockComponent->SetRelativeTransform(FTransform(FRotator::ZeroRotator, BlockLocation, BlockScale));
		}
	}
}

void AP13Blocks::OnBallHitHandle(UPrimitiveComponent* MyComp, AActor* Other)
{
	const AP13Ball* Ball = Cast<AP13Ball>(Other);
	if (!Ball)
	{
		return;
	}
	MyComp->DestroyComponent();
}
