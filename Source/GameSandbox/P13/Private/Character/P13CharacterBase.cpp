// Copyright © 2024, IKinder

#include "P13/Public/Character/P13CharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "Curves/CurveLinearColor.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P13/Public/Component/Actor/P13CharacterAttributesComponent.h"
#include "P13/Public/Component/Actor/P13InventoryComponent.h"
#include "P13/Public/Component/Scene/P13DamageDisplayComponent.h"
#include "P13/Public/Game/P13GameInstance.h"
#include "P13/Public/Weapon/P13ProjectileDefault.h"
#include "P13/Public/Weapon/P13Weapon.h"

AP13CharacterBase::AP13CharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	SetCanBeDamaged(true);

	CreateBaseComponents();
}

void AP13CharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (InventoryComponent)
	{
		InventoryComponent->OnCurrentWeaponUpdated.AddUObject(this, &ThisClass::InitWeapon);
	}
}

void AP13CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ControllerCached = NewController;
	UpdateInventoryAfterRespawn();
}

float AP13CharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	TakeStateEffectFromRadialDamage(DamageEvent, DamageCauser);
	PlayTakeDamageEffect(DamageCauser);

	return ActualDamage;
}

void AP13CharacterBase::UnPossessed()
{
	ControllerCached.Reset();
	Super::UnPossessed();
}

void AP13CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	CreateDynamicMeshMaterials();

	if (AttributesComponent)
	{
		AttributesComponent->OnShieldChanged.AddUObject(this, &ThisClass::OnShieldChangedHandle);
		AttributesComponent->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChangedHandle);
		AttributesComponent->OnHealthOver.AddUObject(this, &ThisClass::OnDeathHandle);
	}
}

EPhysicalSurface AP13CharacterBase::GetSurfaceType()
{
	const UMaterialInterface* MeshMaterial = GetMesh()->GetMaterial(0);
	if (!MeshMaterial)
	{
		return SurfaceType_Default;
	}

	const UPhysicalMaterial* PhysMaterial = MeshMaterial->GetPhysicalMaterial();
	if (!PhysMaterial)
	{
		return SurfaceType_Default;
	}

	return PhysMaterial->SurfaceType;
}

bool AP13CharacterBase::GetCanApplyStateEffect(const TSubclassOf<UP13StateEffect> StateEffectClass) const
{
	check(AttributesComponent)

	const bool bNoShield = !AttributesComponent->GetShieldIsActive();
	const bool bStackable = StateEffectClass && StateEffectClass.GetDefaultObject()->GetIsStackable();
	const bool bUnique = !ActiveStateEffects.FindByPredicate([&](const UP13StateEffect* CompareEffect)
	{
		return CompareEffect->GetClass() == StateEffectClass;
	});

	return bNoShield && (bStackable || bUnique);
}

void AP13CharacterBase::AddActiveStateEffect(UP13StateEffect* StateEffect)
{
	if (!StateEffect)
	{
		return;
	}
	ActiveStateEffects.Add(StateEffect);
}

void AP13CharacterBase::RemoveInactiveStateEffect(UP13StateEffect* InactiveStateEffect)
{
	if (!InactiveStateEffect || !ActiveStateEffects.Contains(InactiveStateEffect))
	{
		return;
	}
	ActiveStateEffects.Remove(InactiveStateEffect);
}

FVector AP13CharacterBase::GetLookAtCursorDirection() const
{
	return GetMesh()->GetForwardVector();
}

float AP13CharacterBase::GetHealthReserve() const
{
	return AttributesComponent->GetCurrentHealth() + AttributesComponent->GetCurrentShield();
}

void AP13CharacterBase::UpdateCharacter() const
{
	float ResSpeed = GetCharacterMovement()->StaticClass()->GetDefaultObject<UCharacterMovementComponent>()->MaxWalkSpeed;

	if (MovementState == EP13MovementState::Aim)
	{
		ResSpeed = MovementSpeed.Aim;
	}
	else if (MovementState == EP13MovementState::Walk)
	{
		ResSpeed = MovementSpeed.Walk;
	}
	else if (MovementState == EP13MovementState::Run)
	{
		ResSpeed = MovementSpeed.Run;
	}
	else if (MovementState == EP13MovementState::Sprint)
	{
		ResSpeed = MovementSpeed.Sprint;
	}

	GetCharacterMovement()->MaxWalkSpeed = ResSpeed;

	if (CachedWeapon.IsValid())
	{
		CachedWeapon->UpdateWeaponState(MovementState);
	}
}

void AP13CharacterBase::ChangeMovementState(const EP13MovementState NewMovementState)
{
	MovementState = NewMovementState;
	UpdateCharacter();
}

void AP13CharacterBase::UpdateInventoryAfterRespawn() const
{
	check(InventoryComponent)
	InventoryComponent->RefreshSlots();
}

void AP13CharacterBase::OnHealthChangedHandle(const float NewHealth, const float LastDamage, const float HealthAlpha)
{
	UpdateMeshMaterial(HealthAlpha);
}

void AP13CharacterBase::OnShieldChangedHandle(const float NewShield, const float LastDamage, const float ShieldAlpha)
{
	check(DamageDisplayComponent)
	DamageDisplayComponent->DisplayShield(LastDamage, ShieldAlpha);
}

void AP13CharacterBase::OnDeathHandle(AController* Causer)
{
	bDead = true;

	/* Drop current weapon and remove it from the inventory. */
	DropWeapon(false);

	if (Controller)
	{
		Controller->UnPossess();
	}

	/* Disable movement. */
	StopAnimMontage();
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->MovementState.bCanJump = false;
	}

	/* Ragdoll. */
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	SetLifeSpan(10.f);
}

void AP13CharacterBase::InitWeapon(const FP13WeaponSlot& NewWeaponSlot, const int32 CurrentIndex)
{
	if (CachedWeapon.IsValid())
	{
		/* Don't do anything if we just picked up another weapon. */
		if (CurrentIndex == CachedWeapon->GetWeaponIndex())
		{
			return;
		}
		CachedWeapon->Destroy();
		CachedWeapon.Reset();
	}
	const UP13GameInstance* GameInstance = GetGameInstance<UP13GameInstance>();
	if (!GameInstance)
	{
		return;
	}
	FP13WeaponInfo* WeaponInfo = GameInstance->GetWeaponInfoByID(NewWeaponSlot.WeaponID);
	if (!WeaponInfo->Class)
	{
		return;
	}

	PlayAnimMontage(WeaponInfo->CharEquipAnim);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	CachedWeapon = GetWorld()->SpawnActor<AP13Weapon>(WeaponInfo->Class, SpawnParams);
	if (!CachedWeapon.IsValid())
	{
		return;
	}
	CachedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
	CachedWeapon->WeaponInit(WeaponInfo, MovementState, CurrentIndex, &NewWeaponSlot.DynamicInfo);
	CachedWeapon->OnWeaponFire.AddUObject(this, &ThisClass::OnWeaponFiredHandle);
	CachedWeapon->OnWeaponReloadInit.AddUObject(this, &ThisClass::OnWeaponReloadInitHandle);
	CachedWeapon->OnWeaponReloadStart.AddUObject(this, &ThisClass::OnWeaponReloadStartHandle);
	CachedWeapon->OnWeaponReloadFinish.AddUObject(this, &ThisClass::OnWeaponReloadFinishHandle);

	CurrentWeaponType = WeaponInfo->AmmoType;
	InventoryComponent->OnSwitchWeapon.Broadcast(CurrentIndex, CachedWeapon.Get());
}

void AP13CharacterBase::DropWeapon(const bool bTakeNext)
{
	CurrentWeaponType = EP13AmmoType::Default;

	check(InventoryComponent)
	if (InventoryComponent->TryDropCurrentWeapon(CachedWeapon.Get(), bTakeNext))
	{
		return;
	}

	if (CachedWeapon.IsValid())
	{
		CachedWeapon->Destroy();
		CachedWeapon.Reset();
	}
}

bool AP13CharacterBase::CheckCharacterCanFire() const
{
	bool bResult = true;
	bResult = bResult && MovementState <= EP13MovementState::Run;

	return bResult;
}

void AP13CharacterBase::OnWeaponFiredHandle(UAnimMontage* CharFireAnim, const int32 CurrentRound)
{
	if (!CachedWeapon.IsValid())
	{
		return;
	}
	PlayAnimMontage(CharFireAnim);
	InventoryComponent->SetWeaponInfo({CurrentRound});
}

void AP13CharacterBase::OnWeaponReloadInitHandle(const int32 OldRoundNum)
{
	check(CachedWeapon.IsValid())
	check(InventoryComponent)

	/* Find max ammo amount that we can reload. */
	const int32 MaxRound = CachedWeapon->GetWeaponInfo()->MaxRound;
	const int32 MagazineSize = InventoryComponent->FindMaxAvailableRound(OldRoundNum, MaxRound);
	CachedWeapon->SetMaxAvailableRound(MagazineSize);
}

void AP13CharacterBase::OnWeaponReloadStartHandle(UAnimMontage* CharReloadAnim, const int32 WeaponIndex, const float ReloadingTime)
{
	PlayAnimMontage(CharReloadAnim);
}

void AP13CharacterBase::OnWeaponReloadFinishHandle(const int32 RoundNum, const int32 WeaponIndex, const bool bSuccess)
{
	StopAnimMontage();
	InventoryComponent->SetWeaponInfo({RoundNum}, true);
}

void AP13CharacterBase::CreateBaseComponents()
{
	InventoryComponent = CreateDefaultSubobject<UP13InventoryComponent>("InventoryActorComponent");
	AttributesComponent = CreateDefaultSubobject<UP13CharacterAttributesComponent>("CharacterAttributesActorComponent");

	DamageDisplayComponent = CreateDefaultSubobject<UP13DamageDisplayComponent>("DamageDisplaySceneComponent");
	DamageDisplayComponent->SetupAttachment(RootComponent);
}

float AP13CharacterBase::GetIKSocketOffset(const FName& VirtualBoneName, const float TraceHalfDistance, const float FromBoneToBottom) const
{
	const FVector SocketLocation = GetMesh()->GetBoneLocation(VirtualBoneName);
	const FVector TraceStart = SocketLocation + FVector(0.f, 0.f, TraceHalfDistance);
	const FVector TraceEnd = SocketLocation - FVector(0.f, 0.f, TraceHalfDistance);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);
	if (HitResult.bBlockingHit)
	{
		return (SocketLocation - HitResult.ImpactPoint).Z - FromBoneToBottom;
	}
	return 0.f;
}

void AP13CharacterBase::LegsIKFloorAlignment()
{
	if (bDead)
	{
		IKLeftLegOffset = 0.f;
		IKRightLegOffset = 0.f;
		IKHitOffset = 0.f;
		return;
	}

	constexpr float MinDistanceThreshold = 12.f;
	constexpr float IKOffsetInterp = 10.f;

	float LeftOffset = GetIKSocketOffset("VB VB SK_Jedihunter_root_l_ankle");
	float RightOffset = GetIKSocketOffset("VB VB SK_Jedihunter_root_r_ankle");
	float HipOffset = 0.f;

	if (FMath::Abs(LeftOffset) > MinDistanceThreshold || FMath::Abs(RightOffset) > MinDistanceThreshold)
	{
		if (LeftOffset > RightOffset)
		{
			HipOffset = LeftOffset;
			LeftOffset = 0.f;
			RightOffset -= HipOffset;
		}
		else
		{
			HipOffset = RightOffset;
			RightOffset = 0.f;
			LeftOffset -= HipOffset;
		}
	}

	IKLeftLegOffset = FMath::FInterpTo(IKLeftLegOffset, LeftOffset, GetWorld()->GetTimeSeconds(), IKOffsetInterp);
	IKRightLegOffset = FMath::FInterpTo(IKRightLegOffset, RightOffset, GetWorld()->GetTimeSeconds(), IKOffsetInterp);
	IKHitOffset = FMath::FInterpTo(GetIKHipOffset(), HipOffset, GetWorld()->GetTimeSeconds(), IKOffsetInterp);
}

void AP13CharacterBase::CreateDynamicMeshMaterials()
{
	for (int32 Index = 1; Index <= 3; ++Index)
	{
		DynamicMaterials.Emplace(GetMesh()->CreateAndSetMaterialInstanceDynamic(Index));
	}
}

void AP13CharacterBase::UpdateMeshMaterial(const float HealthAlpha)
{
	if (!ColorOverLife)
	{
		return;
	}
	for (auto* DynamicMaterial : DynamicMaterials)
	{
		DynamicMaterial->SetVectorParameterValue("MainColor", ColorOverLife->GetLinearColorValue(HealthAlpha));
		DynamicMaterial->SetVectorParameterValue("PaintColor", ColorOverLife->GetLinearColorValue(HealthAlpha));
	}
}

void AP13CharacterBase::TakeStateEffectFromRadialDamage(FDamageEvent const& DamageEvent, AActor* DamageCauser)
{
	/* Check if current damage is radial. */
	if (!DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		return;
	}
	const AP13ProjectileDefault* Projectile = Cast<AP13ProjectileDefault>(DamageCauser);
	if (!Projectile)
	{
		return;
	}
	if (!GetCanApplyStateEffect(Projectile->GetBulletStateEffect()))
	{
		return;
	}
	UP13Types::AddEffectBySurfaceType(this, Projectile->GetBulletStateEffect(), GetSurfaceType(), DamageCauser->GetInstigatorController());
}

void AP13CharacterBase::PlayTakeDamageEffect(const AActor* DamageCauser)
{
	if (ControllerCached.IsValid())
	{
		ControllerCached->StopMovement();
	}

	const FVector CauserLocation = DamageCauser ? DamageCauser->GetActorLocation() : GetActorLocation() + GetActorForwardVector();
	const FVector ShotDir = (CauserLocation  - GetActorLocation()).GetSafeNormal2D();
	const FVector ActorDir = GetMesh()->GetForwardVector();
	const float ShootAngle = FMath::RadiansToDegrees(FMath::Acos(ActorDir | ShotDir)) * FMath::Sign((ActorDir ^ ShotDir).Z);
	
	FString MontageSection = "From";
	if      (ShootAngle >  -45.f && ShootAngle <  45.f)  MontageSection += "Front";
	else if (ShootAngle > -135.f && ShootAngle < -45.f)  MontageSection += "Left";
	else if (ShootAngle >  135.f && ShootAngle < -135.f) MontageSection += "Back";
	else if (ShootAngle >   45.f && ShootAngle <  135.f) MontageSection += "Right";
	
	PlayAnimMontage(TakeDamageAnim, 1.f, *MontageSection);
}