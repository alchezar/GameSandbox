// Copyright © 2024, IKinder

#include "P13/Public/Actor/P13PickupAid.h"

#include "P13/Public/Component/Actor/P13CharacterAttributesComponent.h"

AP13PickupAid::AP13PickupAid()
{
	Mesh->SetSimulatePhysics(false);
}

void AP13PickupAid::BeginPlay()
{
	Super::BeginPlay();
}

void AP13PickupAid::OnCollisionBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCollisionBeginOverlapHandle(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UP13CharacterAttributesComponent* AttributesComponent = OtherActor->FindComponentByClass<UP13CharacterAttributesComponent>();
	if (!AttributesComponent || AttributesComponent->GetIsHealthFull())
	{
		return;
	}
	AttributesComponent->AddHealth(HealthAid);

	OnPickupSuccess();
}
