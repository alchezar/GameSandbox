// Copyright © 2024, IKinder

#include "P13/Public/Character/P13CharacterBase.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Engine/ActorChannel.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "P13/Public/Component/Actor/P13CharacterAttributesComponent.h"
#include "P13/Public/Component/Actor/P13InventoryComponent.h"
#include "P13/Public/Component/Actor/P13LegAlignmentComponent.h"
#include "P13/Public/Component/Scene/P13DamageDisplayComponent.h"
#include "P13/Public/Game/P13GameInstance.h"
#include "P13/Public/Weapon/P13ProjectileDefault.h"
#include "P13/Public/Weapon/P13Weapon.h"

AP13CharacterBase::AP13CharacterBase()
{
	bReplicates = true;
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
		InventoryComponent->OnCurrentWeaponUpdated.AddUObject(this, &ThisClass::Server_InitWeapon);
	}
}

bool AP13CharacterBase::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	bWroteSomething |= Channel->ReplicateSubobject(AttributesComponent, *Bunch, *RepFlags);
	return bWroteSomething;
}

void AP13CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	TryCreateDynamicMeshMaterials();

	if (HasAuthority() && AttributesComponent)
	{
		AttributesComponent->OnShieldChanged.AddUObject(this, &ThisClass::OnShieldChangedHandle);
		AttributesComponent->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChangedHandle);
		AttributesComponent->OnHealthOver.AddUObject(this, &ThisClass::OnDeathHandle);
	}
	if (LegAlignmentComponent)
	{
		LegAlignmentComponent->InitLegAlignment("VB VB SK_Jedihunter_root_l_ankle", "VB VB SK_Jedihunter_root_r_ankle");
	}
}

void AP13CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ControllerCached = NewController;
	UpdateInventoryAfterRespawn();
	TryLoadSavedColor(NewController);
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
	check(StateEffectClass)

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

	if (const UP13SingleStateEffect* EffectToAdd = Cast<UP13SingleStateEffect>(StateEffect))
	{
		UNiagaraSystem* Particle = EffectToAdd->GetParticle();
		if (!Particle)
		{
			return;
		}
		Multicast_AddStateParticle(Particle, EffectToAdd->GetParticleScale(), EffectToAdd->GetIsAutoDestroy());
	}
}

void AP13CharacterBase::RemoveInactiveStateEffect(UP13StateEffect* InactiveStateEffect)
{
	if (!InactiveStateEffect || !ActiveStateEffects.Contains(InactiveStateEffect))
	{
		return;
	}
	ActiveStateEffects.Remove(InactiveStateEffect);
	
	Multicast_RemoveStateParticle();
}

bool AP13CharacterBase::GetIsDead() const
{
	return !(AttributesComponent->GetIsAlive());
}

FVector AP13CharacterBase::GetLookAtCursorDirection() const
{
	return GetMesh()->GetForwardVector();
}

float AP13CharacterBase::GetHealthReserve() const
{
	return AttributesComponent->GetCurrentHealth() + AttributesComponent->GetCurrentShield();
}

bool AP13CharacterBase::TryCreateDynamicMeshMaterials()
{
	if (!DynamicMaterials.IsEmpty())
	{
		return false;
	}

	for (int32 Index = 1; Index <= 3; ++Index)
	{
		DynamicMaterials.Emplace(GetMesh()->CreateAndSetMaterialInstanceDynamic(Index));
	}
	return true;
}

void AP13CharacterBase::UpdateDynamicMeshMaterials(const FLinearColor NewColor)
{
	TrueColor = NewColor;
	for (auto* DynamicMaterial : DynamicMaterials)
	{
		DynamicMaterial->SetVectorParameterValue("MainColor", TrueColor);
		DynamicMaterial->SetVectorParameterValue("PaintColor", TrueColor);
	}
}

void AP13CharacterBase::PlayReadyAnimation()
{
	Multicast_PlayAnimation(ReadyMontage);
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

	if (CachedWeapon.IsValid() && HasAuthority())
	{
		CachedWeapon->UpdateWeaponState(MovementState);
	}
}

void AP13CharacterBase::ChangeMovementState(const EP13MovementState NewMovementState)
{
	MovementState = NewMovementState;
	OnRep_MovementState();
}

void AP13CharacterBase::UpdateInventoryAfterRespawn()
{
	check(InventoryComponent)
	InventoryComponent->Server_RefreshSlots();
}

void AP13CharacterBase::OnHealthChangedHandle(const float NewHealth, const float LastDamage, const float HealthAlpha)
{
	// UpdateMeshMaterial(HealthAlpha);
}

void AP13CharacterBase::OnShieldChangedHandle(const float NewShield, const float LastDamage, const float ShieldAlpha)
{
	check(DamageDisplayComponent)
	DamageDisplayComponent->Server_DisplayShield(LastDamage, ShieldAlpha);
}

void AP13CharacterBase::OnDeathHandle(AController* Causer)
{
	/* Must executes always on server! */
	
	/* Drop current weapon and remove it from the inventory. */
	DropWeapon(false);
	
	if (Controller)
	{
		constexpr float UnpossessDelay = 1.f;
		FTimerHandle RespawnTimer;
		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, Controller.Get(), &AController::UnPossess, UnpossessDelay);
	}
	
	Multicast_StopAnimation();	
	Multicast_OnDeadHandle();
}

void AP13CharacterBase::InitWeapon(const FP13WeaponSlot& NewWeaponSlot, const int32 CurrentIndex)
{
	/* On Server. */
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

	Multicast_PlayAnimation(WeaponInfo->CharEquipAnim);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	CachedWeapon = GetWorld()->SpawnActor<AP13Weapon>(WeaponInfo->Class, SpawnParams);
	if (!CachedWeapon.IsValid())
	{
		return;
	}

	CachedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
	CachedWeapon->WeaponInit(WeaponInfo, MovementState, CurrentIndex, &NewWeaponSlot.DynamicInfo);
	CachedWeapon->OnWeaponFire.AddUObject(this, &ThisClass::OnWeaponFiredHandle);
	CachedWeapon->OnWeaponReloadInit.AddUObject(this, &ThisClass::OnWeaponReloadInitHandle);
	CachedWeapon->OnWeaponReloadStart.AddUObject(this, &ThisClass::OnWeaponReloadStartHandle);
	CachedWeapon->OnWeaponReloadFinish.AddUObject(this, &ThisClass::OnWeaponReloadFinishHandle);

	CurrentWeaponType = WeaponInfo->AmmoType;
	InventoryComponent->Multicast_OnSwitchWeaponBroadcast(CurrentIndex, CachedWeapon.Get());
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

void AP13CharacterBase::PullTrigger(const bool bStart) const
{
	/* Check  if there are any reasons why the character can't pull the trigger. */
	if (!CachedWeapon.IsValid() || !CheckCharacterCanFire())
	{
		return;
	}
	CachedWeapon->SetFireState(bStart);
}

bool AP13CharacterBase::TryReloadWeapon() const
{
	if (!CachedWeapon.IsValid())
	{
		return false;
	}
	return CachedWeapon->TryReload();
}

bool AP13CharacterBase::TryTakeNextWeapon(const bool bNext) const
{
	check(InventoryComponent)
	if (InventoryComponent->GetWeaponSlotsCount() < 2)
	{
		return false;
	}
	if (!CachedWeapon.IsValid())
	{
		return false;
	}
	CachedWeapon->AbortReloading();

	const FP13WeaponDynamicInfo OndInfo = CachedWeapon->GetDynamicInfo();
	const int32 OldIndex = InventoryComponent->GetCurrentWeaponIndex();
	const int32 NextDirection = bNext ? 1 : -1;
	const int32 NewIndex = OldIndex + NextDirection;

	return InventoryComponent->TrySwitchWeaponToIndex(NewIndex, OldIndex, OndInfo, bNext);
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
	Multicast_StopAnimation();
	InventoryComponent->SetWeaponInfo({RoundNum}, true);
}

void AP13CharacterBase::TryLoadSavedColor(AController* NewController)
{
	TryCreateDynamicMeshMaterials();
	UpdateDynamicMeshMaterials(TrueColor);
}

void AP13CharacterBase::CreateBaseComponents()
{
	InventoryComponent = CreateDefaultSubobject<UP13InventoryComponent>("InventoryActorComponent");
	AttributesComponent = CreateDefaultSubobject<UP13CharacterAttributesComponent>("CharacterAttributesActorComponent");
	LegAlignmentComponent = CreateDefaultSubobject<UP13LegAlignmentComponent>("LegAlignmentActorComponent");

	DamageDisplayComponent = CreateDefaultSubobject<UP13DamageDisplayComponent>("DamageDisplaySceneComponent");
	DamageDisplayComponent->SetupAttachment(RootComponent);
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
	const FVector ShotDir = (CauserLocation - GetActorLocation()).GetSafeNormal2D();
	const FVector ActorDir = GetMesh()->GetForwardVector();
	const float ShootAngle = FMath::RadiansToDegrees(FMath::Acos(ActorDir | ShotDir)) * FMath::Sign((ActorDir ^ ShotDir).Z);

	FString MontageSection = "From";
	if      (ShootAngle >  -45.f && ShootAngle <  45.f)  MontageSection += "Front";
	else if (ShootAngle > -135.f && ShootAngle < -45.f)  MontageSection += "Left";
	else if (ShootAngle >  135.f && ShootAngle < -135.f) MontageSection += "Back";
	else if (ShootAngle >   45.f && ShootAngle <  135.f) MontageSection += "Right";
	
	PlayAnimMontage(TakeDamageAnim, 1.f, *MontageSection);
}

void AP13CharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TrueColor)
	DOREPLIFETIME(ThisClass, MovementState)
	DOREPLIFETIME(ThisClass, CurrentWeaponType)
	DOREPLIFETIME(ThisClass, CachedWeapon)
	DOREPLIFETIME(ThisClass, ActiveStateEffects)
}

void AP13CharacterBase::Multicast_UpdatePlayerColor_Implementation(const FLinearColor NewColor)
{
	UpdateDynamicMeshMaterials(NewColor);
}

void AP13CharacterBase::Server_ChangeMovementState_Implementation(const EP13MovementState NewMovementState)
{
	ChangeMovementState(NewMovementState);
}

void AP13CharacterBase::OnRep_MovementState()
{
	UpdateCharacter();
}

void AP13CharacterBase::Multicast_StopAnimation_Implementation(UAnimMontage* Anim)
{
	StopAnimMontage(Anim);
}

void AP13CharacterBase::Server_InitWeapon_Implementation(const FP13WeaponSlot& NewWeaponSlot, const int32 CurrentIndex)
{
	InitWeapon(NewWeaponSlot, CurrentIndex);
}

void AP13CharacterBase::Multicast_PlayAnimation_Implementation(UAnimMontage* Anim)
{
	PlayAnimMontage(Anim);
}

void AP13CharacterBase::Multicast_AddStateParticle_Implementation(UNiagaraSystem* Particle, const float ParticleScale, const bool bAutoDestroy)
{
	/* State effect has already spawned particles on the server. */
	if (HasAuthority() || ParticleComponent)
	{
		return;
	}
	/* Or make an array of particle components, if we need more than one particles at the same time. */
	ParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		Particle,
		GetRootComponent(),
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		FVector(ParticleScale),
		EAttachLocation::SnapToTarget,
		bAutoDestroy,
		ENCPoolMethod::None);
}

void AP13CharacterBase::Multicast_RemoveStateParticle_Implementation()
{
	if (HasAuthority() || !ParticleComponent)
	{
		return;
	}
	ParticleComponent->DestroyComponent();
	ParticleComponent = nullptr;
}

void AP13CharacterBase::Multicast_OnDeadHandle_Implementation()
{
	/* Stop leg alignment. */
	check(LegAlignmentComponent)
	LegAlignmentComponent->LegAlignment(false);

	/* Disable movement. */
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

void AP13CharacterBase::Server_DropWeapon_Implementation(const bool bTakeNext)
{
	DropWeapon(bTakeNext);
}
