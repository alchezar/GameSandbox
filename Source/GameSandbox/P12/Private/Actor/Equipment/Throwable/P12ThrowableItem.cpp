// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Equipment/Throwable/P12ThrowableItem.h"

#include "P12/Public/Actor/Projectile/P12Projectile.h"
#include "P12/Public/Component/Actor/P12EquipmentComponent.h"
#include "P12/Public/Player/P12BaseCharacter.h"
#include "P12/Public/Player/AnimNotify/P12AnimNotify_Thrown.h"

AP12ThrowableItem::AP12ThrowableItem()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");

	ProjectileBillet = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileBilletStaticMeshComponent");
	ProjectileBillet->SetupAttachment(RootComponent);
}

void AP12ThrowableItem::BeginPlay()
{
	Super::BeginPlay();

	OnEquipmentStateChanged.AddUObject(this, &ThisClass::OnEquipmentStateChangedHandle);
}

void AP12ThrowableItem::InitAnimNotify()
{
	Super::InitAnimNotify();

	if (!CharacterEquipAnimMontage)
	{
		return;
	}
	TArray<FAnimNotifyEvent> NotifyEvents = CharacterEquipAnimMontage->Notifies;
	for (FAnimNotifyEvent NotifyEvent : NotifyEvents)
	{
		if (UP12AnimNotify_Thrown* AnimNotifyWindow = Cast<UP12AnimNotify_Thrown>(NotifyEvent.NotifyStateClass))
		{
			AnimNotifyWindow->OnThrowableTaken.AddUObject(this, &ThisClass::OnThrowableTakenHandle);
			AnimNotifyWindow->OnThrowableLaunched.AddUObject(this, &ThisClass::OnThrowableLaunchedHandle);
		}
	}
}

void AP12ThrowableItem::OnThrowableTakenHandle(USkeletalMeshComponent* SkeletalMeshComponent)
{
	if (GetCachedCharacter()->GetMesh() != SkeletalMeshComponent)
	{
		return;
	}
	GetCachedEquipment()->TakeCurrentThrowableItem();
}

void AP12ThrowableItem::OnThrowableLaunchedHandle(USkeletalMeshComponent* SkeletalMeshComponent)
{
	if (GetCachedCharacter()->GetMesh() != SkeletalMeshComponent)
	{
		return;
	}
	GetCachedEquipment()->LaunchCurrentThrowableItem();
}

void AP12ThrowableItem::TakeThrowable()
{
	ProjectileBillet->AttachToComponent(GetCachedCharacter()->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketCharacterWeapon);
}

void AP12ThrowableItem::Throw()
{
	ProjectileBillet->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	const FRotator PlayerRotation = GetCachedCharacter()->GetViewRotation();
	const FRotator FixedRotation = PlayerRotation + FRotator(GravityCompensationAngle, 0.f, 0.f);
	const FVector PlayerDirection = PlayerRotation.RotateVector(FVector::ForwardVector);
	const FVector SpawnLocation = GetCachedCharacter()->GetMesh()->GetSocketLocation(SocketCharacterWeapon) + PlayerDirection * 20.f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	if (AP12Projectile* ThrowableProjectile = GetWorld()->SpawnActor<AP12Projectile>(ProjectileClass, FTransform(FixedRotation, SpawnLocation), SpawnParams))
	{
		ThrowableProjectile->LaunchProjectile(PlayerDirection, this);
	}
}

void AP12ThrowableItem::OnEquipmentStateChangedHandle(const bool bEquipped)
{
	
}
