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

	// CreateMesh();
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

// void ALS_Slicable::CreateMesh()
// {
// 	TArray<FVector> Vertices;
// 	TArray<int32> Triangles;
// 	TArray<FVector> Normals;
// 	TArray<FVector2D> UVs;
// 	TArray<FProcMeshTangent> Tangents;
// 	const TArray<FColor> VertexColors;
//
// 	UKismetProceduralMeshLibrary::GenerateBoxMesh(BoxExtents, Vertices, Triangles, Normals, UVs, Tangents);
// 	SlicableComp->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
// }

void ALS_Slicable::Slice(const FVector& PlanePosition, const FVector& PlaneNormal, UMaterialInterface* CapMaterial)
{
	UProceduralMeshComponent* SlicedComp;
	UKismetProceduralMeshLibrary::SliceProceduralMesh(
		SlicableComp, PlanePosition, PlaneNormal, true, SlicedComp, EProcMeshSliceCapOption::CreateNewSectionForCap, CapMaterial);
	if (!SlicedComp) return;
	
	SlicedComp->SetSimulatePhysics(true);
	SlicedComp->AddImpulse(FVector(300.f), NAME_None, true);

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	UKismetProceduralMeshLibrary::GetSectionFromProceduralMesh(SlicedComp, 0, Vertices, Triangles, Normals, UVs, Tangents);
	SlicedComp->AddCollisionConvexMesh(Vertices);
}
