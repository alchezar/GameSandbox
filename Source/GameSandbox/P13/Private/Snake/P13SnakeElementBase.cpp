// Copyright © 2024, IKinder

#include "P13/Public/Snake/P13SnakeElementBase.h"

#include "P13/Public/Snake/P13SnakeBase.h"

AP13SnakeElementBase::AP13SnakeElementBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SnakeElementMesh = CreateDefaultSubobject<UStaticMeshComponent>("SnakeElementStaticMeshComponent");
	SnakeElementMesh->SetupAttachment(GetRootComponent());
	SnakeElementMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SnakeElementMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AP13SnakeElementBase::BeginPlay()
{
	Super::BeginPlay();
	SnakeElementMesh->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSnakeElementBeginOverlapHandle);
}

void AP13SnakeElementBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP13SnakeElementBase::Respond(AActor* Interactor, const bool bHead)
{
	if (Interactor && Interactor->IsA<AP13SnakeBase>())
	{
		AP13SnakeBase* SnakeBase = StaticCast<AP13SnakeBase*>(Interactor);
		SnakeBase->Destroy();
	}
}

void AP13SnakeElementBase::ToggleCollision()
{
	const bool bCollisionEnabled = SnakeElementMesh->GetCollisionEnabled() == ECollisionEnabled::QueryOnly;
	SnakeElementMesh->SetCollisionEnabled(bCollisionEnabled ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryOnly);
}

void AP13SnakeElementBase::OnSnakeElementBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnSnakeElementBeginOverlap.Broadcast(this, OtherActor);
	/* -> AP13SnakeBase::OnSnakeElementOverlap(...) */
}
