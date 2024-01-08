// Copyright © 2024, IKinder

#include "P13/Public/Actor/P13PickupBase.h"

#include "Components/SphereComponent.h"
#include "P13/Public/Component/Actor/P13InventoryComponent.h"

AP13PickupBase::AP13PickupBase()
{
	CreateComponents();
}

void AP13PickupBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCollisionBeginOverlapHandle);
}

void AP13PickupBase::BeginPlay()
{
	Super::BeginPlay();
}

void AP13PickupBase::OnCollisionBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AP13PickupBase::OnPickupSuccess()
{
	Destroy();
}

void AP13PickupBase::CreateComponents()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	SetRootComponent(SceneRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupStaticMeshComponent");
	Mesh->SetupAttachment(SceneRoot);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetSimulatePhysics(true);
	
	Collision = CreateDefaultSubobject<USphereComponent>("CollisionSphereComponent");
	Collision->SetupAttachment(SceneRoot);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}
