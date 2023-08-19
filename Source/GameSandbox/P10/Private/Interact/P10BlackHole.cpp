// Copyright (C) 2023, IKinder

#include "P10/Public/Interact/P10BlackHole.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

AP10BlackHole::AP10BlackHole()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SceneRootComponent = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	SetRootComponent(SceneRootComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BlackHoleMeshComponent");
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetWorldScale3D(FVector(0.37f));
	
	InnerSphereComponent = CreateDefaultSubobject<USphereComponent>("InnerSphereCollisionComponent");
	InnerSphereComponent->SetupAttachment(RootComponent);

	OuterSphereComponent = CreateDefaultSubobject<USphereComponent>("OuterSphereCollisionComponent");
	OuterSphereComponent->SetupAttachment(RootComponent);
	OuterSphereComponent->SetSphereRadius(OuterSphereRadius);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("BlackHoleParticleSystemComponent");
	ParticleSystemComponent->SetupAttachment(RootComponent);
	ParticleSystemComponent->SetRelativeLocation(FVector(0.f, 0.f, 68.f));
	ParticleSystemComponent->SetRelativeRotation(FRotator(0.f, 0.f, 180.f));
	ParticleSystemComponent->SetTemplate(BlackHoleEffect);
}

void AP10BlackHole::BeginPlay()
{
	Super::BeginPlay();
	InnerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlapHandle);
}

void AP10BlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bActive) return;
	
	TArray<UPrimitiveComponent*> OverlappingComponents;
	OuterSphereComponent->GetOverlappingComponents(OverlappingComponents);
	for(UPrimitiveComponent* OverlappingComponent : OverlappingComponents)
	{
		if (!OverlappingComponent || !OverlappingComponent->IsSimulatingPhysics()) continue;
		OverlappingComponent->AddRadialForce(GetActorLocation(), OuterSphereRadius, -1.f * AttractionForce, RIF_Constant, true);
	}
}

void AP10BlackHole::OnSphereBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !OtherComp) return;
	
	constexpr float MinScale = 0.5f;
	if (OtherActor->GetActorScale3D().GetMin() < MinScale)
	{
		OtherActor->Destroy();
		return;
	}
	
	OtherActor->SetActorScale3D(OtherActor->GetActorScale3D() * 0.9f);
	OtherComp->AddRadialImpulse(GetActorLocation(), OuterSphereRadius, -0.1f * AttractionForce, RIF_Constant, true);
}
