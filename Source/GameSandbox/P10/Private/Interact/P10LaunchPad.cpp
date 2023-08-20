// Copyright (C) 2023, IKinder

#include "P10/Public/Interact/P10LaunchPad.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

AP10LaunchPad::AP10LaunchPad()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	SetRootComponent(SceneRoot);

	PadMesh = CreateDefaultSubobject<UStaticMeshComponent>("PadStaticMeshComponent");
	PadMesh->SetupAttachment(RootComponent);
	PadMesh->SetRelativeLocation(FVector(0.f, 0.f, 5.f));

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollisionComponent");
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetRelativeLocation(FVector(0.f, 0.f, 30.f));

	LaunchDirection = CreateDefaultSubobject<UArrowComponent>("LaunchDirectionArrowComponent");
	LaunchDirection->SetupAttachment(RootComponent);
}

void AP10LaunchPad::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	LaunchDirection->SetRelativeRotation(FRotator(LaunchAngle, 0.f, 0.f));
}

void AP10LaunchPad::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxBeginOverlapHandle);

	// LaunchVelocity = (GetActorForwardVector() + GetActorUpVector()) * LaunchPower;
	FRotator LaunchRotation = GetActorRotation();
	LaunchRotation.Pitch = LaunchAngle;
	LaunchVelocity = LaunchRotation.Vector() * LaunchPower;
}

void AP10LaunchPad::OnBoxBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/* When this is a physics simulated object - add impulse. */
	if (OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVelocity);
		return;
	}
	/* When this is a character - launch them. */
	if (ACharacter* Char = Cast<ACharacter>(OtherActor))
	{
		Char->LaunchCharacter(LaunchVelocity, true, true);
	}
}
