// Copyright (C) 2023, IKinder

#include "P12/Public/Subsystem/Streaming/P12StreamingSubsystemVolume.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "P12/Public/Subsystem/Streaming/P12StreamingSubsystem.h"
#include "P12/Public/Subsystem/Streaming/P12StreamingSubsystemManager.h"
#include "P12/Public/Subsystem/Streaming/P12StreamingSubsystemUtils.h"

AP12StreamingSubsystemVolume::AP12StreamingSubsystemVolume()
{
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	SetRootComponent(SceneRoot);

	Collision = CreateDefaultSubobject<UBoxComponent>("CollisionBoxComponent");
	Collision->SetupAttachment(SceneRoot);
}

void AP12StreamingSubsystemVolume::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("AP12StreamingSubsystemVolume::BeginPlay(): %s"), *GetNameSafe(this));

	StreamingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UP12StreamingSubsystem>();

	Collision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBeginOverlapHandle);
	Collision->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionEndOverlapHandle);
	Collision->SetCollisionResponseToAllChannels(ECR_Overlap);

	UP12StreamingSubsystemUtils::CheckStreamingSubsystemVolumeOverlapCharacter(this);
}

void AP12StreamingSubsystemVolume::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("AP12StreamingSubsystemVolume::EndPlay(): %s"), *GetNameSafe(this));

	Collision->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnCollisionBeginOverlapHandle);
	Collision->OnComponentEndOverlap.RemoveDynamic(this, &ThisClass::OnCollisionEndOverlapHandle);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);

	if (OverlappedCharacter.IsValid())
	{
		OverlappedCharacter.Reset();
		StreamingSubsystem->OnVolumeOverlapEnd(this);
	}
	StreamingSubsystem.Reset();

	Super::EndPlay(EndPlayReason);
}

void AP12StreamingSubsystemVolume::OnCollisionBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("AP12StreamingSubsystemVolume::EndPlay(): %s, OtherActor: %s"), *GetNameSafe(this), *GetNameSafe(OtherActor));

	if (!OtherActor)
	{
		return;
	}
	HandleCharacterOverlapBegin(Cast<ACharacter>(OtherActor));
}

void AP12StreamingSubsystemVolume::OnCollisionEndOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("AP12StreamingSubsystemVolume::EndPlay(): %s, OtherActor: %s"), *GetNameSafe(this), *GetNameSafe(OtherActor));

	if (!StreamingSubsystem.IsValid() || !StreamingSubsystem->GetCanUseSubsystem())
	{
		return;
	}
	const ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!Character || !Character->IsPlayerControlled())
	{
		return;
	}
	if (OverlappedCharacter != Character)
	{
		return;
	}
	OverlappedCharacter.Reset();
	StreamingSubsystem->OnVolumeOverlapEnd(this);
}

void AP12StreamingSubsystemVolume::HandleCharacterOverlapBegin(ACharacter* Character)
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("AP12StreamingSubsystemVolume::HandleCharacterOverlapBegin(): %s, Character: %s"), *GetNameSafe(this), *GetNameSafe(Character));

	if (!StreamingSubsystem.IsValid() || !StreamingSubsystem->GetCanUseSubsystem())
	{
		return;
	}
	if (!Character || !Character->IsPlayerControlled())
	{
		return;
	}
	if (OverlappedCharacter.IsValid())
	{
		return;
	}

	OverlappedCharacter = Character;
	StreamingSubsystem->OnVolumeBeginOverlap(this);
}
