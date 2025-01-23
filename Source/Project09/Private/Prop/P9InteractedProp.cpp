// Copyright (C) 2023, IKinder

#include "Prop/P9InteractedProp.h"

AP9InteractedProp::AP9InteractedProp()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshComponent");
	BaseMesh->SetupAttachment(RootComponent);
}

void AP9InteractedProp::BeginPlay()
{
	Super::BeginPlay();
}

void AP9InteractedProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP9InteractedProp::Interact(ACharacter* Causer)
{
	if (!bCanScale) return;
	bBig = !bBig;

	GetWorld()->GetTimerManager().SetTimer(ScaleTimer, this, &ThisClass::ScaleMesh, GetWorld()->GetDeltaSeconds(), true);
}

void AP9InteractedProp::ScaleMesh()
{
	const FVector TargetScale = bBig ? FVector(2.f) : FVector(1.f);
	BaseMesh->SetWorldScale3D(FMath::Lerp(BaseMesh->GetComponentScale(), TargetScale, 0.05f));
	
	if (FMath::IsNearlyEqual(BaseMesh->GetComponentScale().Size(), TargetScale.Size(), 0.1f))
	{
		GetWorld()->GetTimerManager().ClearTimer(ScaleTimer);
	}
}

