// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Interactive/P12InteractiveActor.h"

#include "Components/CapsuleComponent.h"
#include "P12/Public/Player/P12BaseCharacter.h"

AP12InteractiveActor::AP12InteractiveActor()
{}

void AP12InteractiveActor::BeginPlay()
{
	Super::BeginPlay();

	if (!InteractionVolume)
	{
		return;
	}
	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnInteractionVolumeBeginOverlap);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnInteractionVolumeEndOverlap);
}

bool AP12InteractiveActor::IsOverlappingCharacterCapsule(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	const AP12BaseCharacter* BaseCharacter = Cast<AP12BaseCharacter>(OtherActor);
	if (!BaseCharacter)
	{
		return false;
	}
	if (OtherComp != BaseCharacter->GetCapsuleComponent())
	{
		return false;
	}
	return true;
}

void AP12InteractiveActor::OnInteractionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
	{
		return;
	}
	AP12BaseCharacter* BaseCharacter = StaticCast<AP12BaseCharacter*>(OtherActor);
	BaseCharacter->RegisterInteractiveActor(this);
}

void AP12InteractiveActor::OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
	{
		return;
	}
	AP12BaseCharacter* BaseCharacter = StaticCast<AP12BaseCharacter*>(OtherActor);
	BaseCharacter->UnregisterInteractiveActor(this);
}
