// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Interactive/P12InteractiveActor.h"

#include "Components/CapsuleComponent.h"
#include "P12/Public/Player/P12BaseCharacter.h"

AP12InteractiveActor::AP12InteractiveActor()
{
}

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

void AP12InteractiveActor::OnInteractionVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AP12BaseCharacter* BaseCharacter = Cast<AP12BaseCharacter>(OtherActor);
	if (!BaseCharacter)
	{
		return;
	}
	if (Cast<UCapsuleComponent>(OtherComp) != BaseCharacter->GetCapsuleComponent())
	{
		return;
	}
	BaseCharacter->RegisterInteractiveActor(this);
}

void AP12InteractiveActor::OnInteractionVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	AP12BaseCharacter* BaseCharacter = Cast<AP12BaseCharacter>(OtherActor);
	if (!BaseCharacter)
	{
		return;
	}
	if (Cast<UCapsuleComponent>(OtherComp) != BaseCharacter->GetCapsuleComponent())
	{
		return;
	}
	BaseCharacter->UnregisterInteractiveActor(this);
	
}

