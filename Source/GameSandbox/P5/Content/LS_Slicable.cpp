// Copyright (C) 2023, IKinder

#include "LS_Slicable.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"

ALS_Slicable::ALS_Slicable()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	ReferenceComp = CreateDefaultSubobject<UStaticMeshComponent>("ReferenceComponent");
	ReferenceComp->SetupAttachment(GetRootComponent());
	ReferenceComp->SetCollisionProfileName("NoCollision");
	ReferenceComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	ReferenceComp->bHiddenInGame = true;

	SlicableComp = CreateDefaultSubobject<UProceduralMeshComponent>("SlicedMeshComponent");
	SlicableComp->SetupAttachment(GetRootComponent());
	SlicableComp->SetCollisionProfileName("BlockAll");
	SlicableComp->SetCollisionResponseToAllChannels(ECR_Block);
	SlicableComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SlicableComp->bUseComplexAsSimpleCollision = false;
	SlicableComp->SetSimulatePhysics(true);

	UKismetProceduralMeshLibrary::CopyProceduralMeshFromStaticMeshComponent(ReferenceComp, 0, SlicableComp, true);
}

void ALS_Slicable::BeginPlay()
{
	Super::BeginPlay();
}

void ALS_Slicable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UProceduralMeshComponent* ALS_Slicable::GetSlicableComp() const
{
	return SlicableComp;
}

void ALS_Slicable::Slice(const FVector& PlanePosition, const FVector& PlaneNormal, UMaterialInterface* CapMaterial)
{
	UProceduralMeshComponent* SlicedComp;
	UKismetProceduralMeshLibrary::SliceProceduralMesh(
		SlicableComp, PlanePosition, PlaneNormal, true, SlicedComp, EProcMeshSliceCapOption::CreateNewSectionForCap, CapMaterial);
	if (!SlicedComp) return;

	SlicedComp->AddRelativeLocation(FVector(0.f, 0.f, 5.f));
	SlicedComp->SetSimulatePhysics(true);
	// SlicedComp->AddImpulse(FVector(300.f), NAME_None, true);
	SlicedComp->SetAllPhysicsLinearVelocity(FVector(300.f));
	
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	
	UKismetProceduralMeshLibrary::GetSectionFromProceduralMesh(SlicedComp, 0, Vertices, Triangles, Normals, UVs, Tangents);
	SlicedComp->AddCollisionConvexMesh(Vertices);
	SlicedComp->SetCollisionProfileName("BlockAll");

	Vertices.Empty();
	Triangles.Empty();
	Normals.Empty();
	UVs.Empty();
	Tangents.Empty();
	
	UKismetProceduralMeshLibrary::GetSectionFromProceduralMesh(SlicableComp, 0, Vertices, Triangles, Normals, UVs, Tangents);
	SlicableComp->AddCollisionConvexMesh(Vertices);
	SlicableComp->SetCollisionProfileName("BlockAll");
}
