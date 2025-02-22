// Copyright (C) 2023, IKinder

#include "Actor/Equipment/Weapon/P12RangeWeaponItem.h"

#include "Component/Actor/P12EquipmentComponent.h"
#include "Component/Scene/P12WeaponBarrelComponent.h"
#include "Player/P12BaseCharacter.h"
#include "Player/AnimNotify/P12AnimNotify_Reloaded.h"
#include "Player/AnimNotify/P12AnimNotify_ReloadOneAmmo.h"

AP12RangeWeaponItem::AP12RangeWeaponItem()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneWeaponRootComponent");

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSkeletalMeshComponent");
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->SetRelativeLocation(WeaponMeshOffset);
	WeaponMesh->SetRelativeRotation(WeaponMeshOrientation);

	WeaponBarrel = CreateDefaultSubobject<UP12WeaponBarrelComponent>("WeaponBarrelSceneComponent");
	WeaponBarrel->SetupAttachment(WeaponMesh, MuzzleSocketName);

	EquippedSocketName = SocketCharacterWeapon;
}

void AP12RangeWeaponItem::BeginPlay()
{
	Super::BeginPlay();

	SetAmmo(MaxAmmo);
}

void AP12RangeWeaponItem::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	ProcessRecoil(DeltaTime);
	ProcessRecoilRollback(DeltaTime);
}

void AP12RangeWeaponItem::FireInput(const bool bStart)
{
	if (!bStart)
	{
		AccumulatedRecoil.RollbackTime = AccumulatedRecoil.Shots * 60.f / RecoilParams.RollbackSpeed;
		if (AccumulatedRecoil.Pitch != 0.f || AccumulatedRecoil.Yaw != 0.f)
		{
			GetWorld()->GetTimerManager().SetTimer(RecoilRollbackTimer, this, &ThisClass::StopRecoilRollback, AccumulatedRecoil.RollbackTime);
		}

		GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
		return;
	}
	StopRecoilRollback();
	MakeShot();
	if (FireMode < EP12FireMode::FullAuto)
	{
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
	GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &ThisClass::MakeShot, GetShotTimeInterval(), true);
}

void AP12RangeWeaponItem::AimInput(const bool bStart)
{
	bAiming = bStart;
}

void AP12RangeWeaponItem::InitAnimNotify()
{
	Super::InitAnimNotify();

	if (!CharacterReloadMontage)
	{
		return;
	}
	TArray<FAnimNotifyEvent> NotifyEvents = CharacterReloadMontage->Notifies;
	for (const FAnimNotifyEvent NotifyEvent : NotifyEvents)
	{
		if (UP12AnimNotify_Reloaded* ReloadedNotify = Cast<UP12AnimNotify_Reloaded>(NotifyEvent.Notify))
		{
			ReloadedNotify->OnFullReloaded.AddUObject(this, &ThisClass::OnFullReloadedHandle);
		}
		if (UP12AnimNotify_ReloadOneAmmo* ReloadOneAmmoNotify = Cast<UP12AnimNotify_ReloadOneAmmo>(NotifyEvent.Notify))
		{
			ReloadOneAmmoNotify->OnOneReload.AddUObject(this, &ThisClass::OnOneReloadedHandle);
		}
	}
}

float AP12RangeWeaponItem::PlayAnimMontage(UAnimMontage* AnimMontage, const float InPlayRate, const FName StartSectionName) const
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	if (!WeaponAnimInstance)
	{
		return 0.f;
	}
	const float Duration = WeaponAnimInstance->Montage_Play(AnimMontage, InPlayRate);
	if (Duration <= 0)
	{
		return 0.f;
	}
	if (StartSectionName != NAME_None)
	{
		WeaponAnimInstance->Montage_JumpToSection(StartSectionName, AnimMontage);
	}
	return Duration;
}

void AP12RangeWeaponItem::StopAnimMontage(UAnimMontage* AnimMontage)
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	if (!WeaponAnimInstance)
	{
		return;
	}
	WeaponAnimInstance->Montage_Stop(AnimMontage->GetBlendOutArgs().BlendTime, AnimMontage);
}

FTransform AP12RangeWeaponItem::GetGripTransform() const
{
	return WeaponMesh->GetSocketTransform("Grip_Socket");
}

float AP12RangeWeaponItem::GetShotTimeInterval()
{
	return 60.f / ShotRate;
}

void AP12RangeWeaponItem::MakeShot()
{
	if (!GetCanShoot())
	{
		FireInput(false);
		if (Ammo == 0 && bAutoReload)
		{
			StartReloading();
		}
		return;
	}
	check(GetCachedCharacter().IsValid())
	GetCachedCharacter() = StaticCast<AP12BaseCharacter*>(GetOwner());

	FVector ShotLocation;
	FRotator ShotRotation;
	if (GetCachedCharacter()->IsPlayerControlled())
	{
		const AController* Controller = GetCachedCharacter()->GetController();
		if (!Controller)
		{
			return;
		}
		Controller->GetPlayerViewPoint(ShotLocation, ShotRotation);
	}
	else
	{
		ShotLocation = WeaponBarrel->GetComponentLocation();
		ShotRotation = GetCachedCharacter()->GetBaseAimRotation();
	}
	const FVector ShotDirection = ShotRotation.RotateVector(FVector::ForwardVector);

	CurrentBulletSpread = GetCachedCharacter()->GetIsAiming() ? AimBulletSpread : BulletSpread;

	SetAmmo(--Ammo);
	WeaponBarrel->Shot(ShotLocation, ShotDirection, CurrentBulletSpread);

	GetCachedCharacter()->PlayAnimMontage(CharacterFireMontage);
	PlayAnimMontage(WeaponFireMontage);

	/* Timer for recoil processing. */
	GetWorld()->GetTimerManager().SetTimer(RecoilTimer, GetRecoilTimeInterval(), false);
	/* ::Tick(...) -> ::ProcessRecoil(...) */

	/* Camera shake on shot. */
	if (ShotCameraShakeClass && GetCachedCharacter()->IsPlayerControlled())
	{
		APlayerController* Controller = GetCachedCharacter()->GetController<APlayerController>();
		if (!Controller)
		{
			return;
		}
		// Controller->ClientStartCameraShake(ShotCameraShakeClass);
		Controller->PlayerCameraManager->StartCameraShake(ShotCameraShakeClass);
	}
	++AccumulatedRecoil.Shots;
}

void AP12RangeWeaponItem::SetAmmo(const int32 NewAmmo)
{
	Ammo = FMath::Clamp(NewAmmo, 0, MaxAmmo);
	RefreshAmmoCount();
}

bool AP12RangeWeaponItem::GetCanShoot()
{
	bool bResult = true;
	bResult &= Ammo > 0;
	bResult &= !bReloading;
	bResult &= !GetCachedCharacter()->GetIsEquipping();

	if (FireMode < EP12FireMode::FullAuto)
	{
		const float CurrentTime = GetWorld()->GetRealTimeSeconds();
		bResult &= (CurrentTime - LastShotTime) >= GetShotTimeInterval();
		if (bResult)
		{
			LastShotTime = CurrentTime;
		}
	}

	return bResult;
}

void AP12RangeWeaponItem::StartReloading()
{
	const bool bAmmoEmpty = GetCachedCharacter()->GetEquipmentComponent()->GetMaxAvailableAmmoAmount(AmmoType) <= 0;
	if (bReloading || bAmmoEmpty)
	{
		return;
	}
	bReloading = true;
	if (!CharacterReloadMontage)
	{
		OnFullReloadedHandle(GetCachedCharacter()->GetMesh());
		return;
	}
	GetCachedCharacter()->PlayAnimMontage(CharacterReloadMontage);
	PlayAnimMontage(WeaponReloadMontage);
	/* ::OnFullReloadedHandle */
}

void AP12RangeWeaponItem::OnFullReloadedHandle(USkeletalMeshComponent* SkeletalMeshComponent)
{
	if (!GetCachedCharacter().IsValid() || GetCachedCharacter()->GetMesh() != SkeletalMeshComponent || !bReloading)
	{
		return;
	}
	Reload();
	FinishReload();
}

void AP12RangeWeaponItem::OnOneReloadedHandle(USkeletalMeshComponent* SkeletalMeshComponent, int32 NumberOfAmmo)
{
	if (GetCachedCharacter()->GetMesh() != SkeletalMeshComponent)
	{
		return;
	}
	if (Ammo == MaxAmmo)
	{
		FinishReload(false);
		return;
	}
	Reload(true, NumberOfAmmo);
}

void AP12RangeWeaponItem::Reload(const bool bByOne, const int32 NumberOfAmmo)
{
	bReloading = false;

	const int32 AvailableAmmoCount = GetEquipment()->GetMaxAvailableAmmoAmount(AmmoType);
	const int32 AmmoToReload = bByOne ? NumberOfAmmo : MaxAmmo - Ammo;
	const int32 ReloadedAmmo = FMath::Min(AvailableAmmoCount, AmmoToReload);
	GetEquipment()->DecreaseMaxAvailableAmmoAmount(AmmoType, ReloadedAmmo);

	SetAmmo(Ammo + ReloadedAmmo);
	GetCachedCharacter()->OnReloadComplete.Broadcast(true);
}

void AP12RangeWeaponItem::FinishReload(const bool bJumpToEnd)
{
	if (CharacterReloadMontage)
	{
		GetCachedCharacter()->StopAnimMontage(CharacterReloadMontage);
	}
	if (WeaponReloadMontage)
	{
		StopAnimMontage(WeaponReloadMontage);
	}

	if (!bJumpToEnd)
	{
		GetCachedCharacter()->PlayAnimMontage(CharacterReloadMontage, 1, MontageSectionName_ReloadEnd);
		PlayAnimMontage(WeaponReloadMontage, 1, MontageSectionName_ReloadEnd);
	}
}

void AP12RangeWeaponItem::AttachItem(const FName AttachSocketName)
{
	Super::AttachItem(AttachSocketName);
	RefreshAmmoCount();
}

void AP12RangeWeaponItem::RefreshAmmoCount() const
{
	const int32 AvailableAmmoCount = GetEquipment()->GetMaxAvailableAmmoAmount(AmmoType);
	GetCachedCharacter()->OnAmmoCountChanged.Broadcast(Ammo, AvailableAmmoCount);
}

UP12EquipmentComponent* AP12RangeWeaponItem::GetEquipment() const
{
	UP12EquipmentComponent* Equipment = GetCachedEquipment().Get();
	if (!Equipment)
	{
		/* For some reason in multiplayer EquipmentComponent does`n cache. So we need to get it from our owner. */
		AActor* CurrentOwner = GetOwner();
		check(CurrentOwner)
		if (!CurrentOwner->IsA<APawn>())
		{
			CurrentOwner = GetOwner()->GetOwner();
		}
		const AP12BaseCharacter* BaseCharacter = Cast<AP12BaseCharacter>(CurrentOwner);
		check(BaseCharacter)
		Equipment = BaseCharacter->GetEquipmentComponent();
	}
	return Equipment;
}

void AP12RangeWeaponItem::OnLevelDeserialized_Implementation()
{
	SetActorRelativeTransform(FTransform::Identity);
	RefreshAmmoCount();
}

float AP12RangeWeaponItem::GetRecoilTimeInterval()
{
	return FMath::Min(RecoilParams.TimeInterval, GetShotTimeInterval());
}

void AP12RangeWeaponItem::ProcessRecoil(const float DeltaTime)
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(RecoilTimer))
	{
		return;
	}
	AP12BaseCharacter* CharacterOwner = GetCachedCharacter().Get();
	if (!CharacterOwner)
	{
		return;
	}
	if (!CharacterOwner->IsPlayerControlled() || !CharacterOwner->IsLocallyControlled())
	{
		return;
	}

	const float RecoilPitch = -RecoilParams.Pitch / GetRecoilTimeInterval() * DeltaTime;
	CharacterOwner->AddControllerPitchInput(RecoilPitch);
	AccumulatedRecoil.Pitch += RecoilPitch;

	float RecoilYaw = RecoilParams.Yaw / GetRecoilTimeInterval() * DeltaTime;
	RecoilYaw = FMath::RandBool() ? RecoilYaw : -RecoilYaw;
	CharacterOwner->AddControllerYawInput(RecoilYaw);
	AccumulatedRecoil.Yaw += RecoilYaw;
}

void AP12RangeWeaponItem::ProcessRecoilRollback(const float DeltaTime)
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(RecoilRollbackTimer))
	{
		return;
	}
	AP12BaseCharacter* CharacterOwner = GetCachedCharacter().Get();
	if (!CharacterOwner)
	{
		return;
	}
	if (!CharacterOwner->IsPlayerControlled() || !CharacterOwner->IsLocallyControlled())
	{
		return;
	}

	const float RollbackPitch = -AccumulatedRecoil.Pitch / AccumulatedRecoil.RollbackTime * DeltaTime;
	CharacterOwner->AddControllerPitchInput(RollbackPitch);
	const float RollbackYaw = -AccumulatedRecoil.Yaw / AccumulatedRecoil.RollbackTime * DeltaTime;
	CharacterOwner->AddControllerYawInput(RollbackYaw);
}

void AP12RangeWeaponItem::StopRecoilRollback()
{
	GetWorld()->GetTimerManager().ClearTimer(RecoilRollbackTimer);
	AccumulatedRecoil.Reset();
}
