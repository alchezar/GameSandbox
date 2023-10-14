// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Interactive/Environment/P12Ladder.h"

#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"

AP12Ladder::AP12Ladder()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	
	RightRailMesh = CreateDefaultSubobject<UStaticMeshComponent>("RightRailStaticMeshComponent");
	RightRailMesh->SetupAttachment(RootComponent);

	LeftRailMesh = CreateDefaultSubobject<UStaticMeshComponent>("LeftRailStaticMeshComponent");
	LeftRailMesh->SetupAttachment(RootComponent);

	StepRailMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("StepRailInstancedStaticMeshComponent");
	StepRailMesh->SetupAttachment(RootComponent);

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>("BoxInteractionVolumeComponent");
	InteractionVolume->SetupAttachment(RootComponent);
}

void AP12Ladder::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	RightRailMesh->SetRelativeLocation(FVector(-0.5f * Width, 0.f, Height / 2));
	LeftRailMesh->SetRelativeLocation(FVector( 0.5f * Width, 0.f, Height / 2));
	StepRailMesh->SetRelativeLocation((BottomOffset / 2.f) * FVector::UpVector);

	const UStaticMesh* RailMesh = RightRailMesh->GetStaticMesh();
	const UStaticMesh* StepMesh = StepRailMesh->GetStaticMesh();
	if (!RailMesh || !StepMesh)
	{
		return;
	}
	const float MeshHeight = RailMesh->GetBoundingBox().GetSize().Z;
	const float MeshWidth = StepMesh->GetBoundingBox().GetSize().Y;
	if (FMath::IsNearlyZero(MeshHeight) || FMath::IsNearlyZero(MeshWidth))
	{
		return;
	}
	
	RightRailMesh->SetRelativeScale3D(FVector(1.f, 1.f, Height / MeshHeight));
	LeftRailMesh->SetRelativeScale3D(FVector(1.f, 1.f, Height / MeshHeight));
	StepRailMesh->SetRelativeScale3D(FVector(1.f, Width / MeshWidth, 1.f));

	StepRailMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	InteractionVolume->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	StepRailMesh->ClearInstances();
	const uint32 StepsCount = FMath::FloorToInt((Height - BottomOffset) / StepInterval);
	for (uint32 i = 0; i < StepsCount; ++i)
	{
		const FTransform StepTransform = FTransform(FVector(1.f, 1.f, BottomOffset + i * StepInterval));
		StepRailMesh->AddInstance(StepTransform);
	}

	const float BoxDepth = GetLadderInteractionBox()->GetUnscaledBoxExtent().X;
	GetLadderInteractionBox()->SetBoxExtent(FVector(BoxDepth, Width / 2.f, Height / 2.f));
	GetLadderInteractionBox()->SetRelativeLocation(FVector(0.f, BoxDepth, Height / 2.f));
}

void AP12Ladder::BeginPlay()
{
	Super::BeginPlay();
}

void AP12Ladder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UBoxComponent* AP12Ladder::GetLadderInteractionBox() const
{
	return StaticCast<UBoxComponent*>(InteractionVolume);
}

