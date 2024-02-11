// Copyright © 2024, IKinder

#include "P13/Public/Actor/P13PickupActor.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "P13/Public/Component/Actor/P13CharacterAttributesComponent.h"
#include "P13/Public/Component/Actor/P13InventoryComponent.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              PickingUp Base                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

AP13PickingUpBase::AP13PickingUpBase()
{
	bReplicates = true;
	CreateComponents();
}

void AP13PickingUpBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCollisionBeginOverlapHandle);
}

void AP13PickingUpBase::BeginPlay()
{
	Super::BeginPlay();
	ActivateParticles();
}

void AP13PickingUpBase::OnPickupSuccess()
{
	if (UNiagaraComponent* PickedParticle = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffect, Mesh->GetComponentLocation()))
	{
		PickedParticle->SetVariableLinearColor("Color", EffectColor);
	}
	Destroy();
}

void AP13PickingUpBase::OnCollisionBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	BePickedUpOnCollisionBeginOverlap(OtherActor);
}

void AP13PickingUpBase::ActivateParticles()
{
	if (UNiagaraComponent* WaitParticle = UNiagaraFunctionLibrary::SpawnSystemAttached(WaitEffect, Mesh, NAME_None, Mesh->GetComponentLocation(), FRotator::ZeroRotator, EAttachLocation::KeepWorldPosition, false))
	{
		WaitParticle->SetVariableLinearColor("Color", EffectColor);
	}
}

void AP13PickingUpBase::BePickedUpOnCollisionBeginOverlap(AActor* Picker)
{
	
}

void AP13PickingUpBase::CreateComponents()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	SetRootComponent(SceneRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupStaticMeshComponent");
	Mesh->SetupAttachment(SceneRoot);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetSimulatePhysics(true);

	Collision = CreateDefaultSubobject<USphereComponent>("CollisionSphereComponent");
	Collision->SetupAttachment(SceneRoot);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              PickingUp Aid                                *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

AP13PickingUpAid::AP13PickingUpAid()
{
	Mesh->SetSimulatePhysics(false);
}

void AP13PickingUpAid::BePickedUpOnCollisionBeginOverlap(AActor* Picker)
{
	UP13CharacterAttributesComponent* AttributesComponent = Picker->FindComponentByClass<UP13CharacterAttributesComponent>();
	if (!AttributesComponent || AttributesComponent->GetIsHealthFull())
	{
		return;
	}
	AttributesComponent->Server_AddHealth(HealthAid);

	OnPickupSuccess();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              PickingUp Ammo                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void AP13PickingUpAmmo::BeginPlay()
{
	Super::BeginPlay();

	Collision->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AP13PickingUpAmmo::BePickedUpOnCollisionBeginOverlap(AActor* Picker)

{
	UP13InventoryComponent* InventoryComponent = Picker->FindComponentByClass<UP13InventoryComponent>();
	if (!InventoryComponent)
	{
		return;
	}
	IP13PickupInterface* PickupInterface = Cast<IP13PickupInterface>(InventoryComponent);
	if (!PickupInterface)
	{
		return;
	}

	/* Try to add ammo to the inventory. */
	PickupInterface->PickupAmmoToInventory(AmmoSlot, this);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                             PickingUp Weapon                              *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void AP13PickingUpWeapon::BeginPlay()
{
	Super::BeginPlay();

	/* If weapon wasn't ever picked up before - magazine will be full. */
	if (!bDropped)
	{
		WeaponSlot.DynamicInfo.Round = WeaponSlot.MaxRound;
	}
}

void AP13PickingUpWeapon::InitDrop(const FP13WeaponDrop* DropWeaponInfo)
{
	/* Save init info. */
	bDropped = true;
	Mesh->SetStaticMesh(DropWeaponInfo->WeaponMesh);
	WeaponSlot = DropWeaponInfo->WeaponInfo;

	WaitEffect = DropWeaponInfo->WaitEffect;
	PickupEffect = DropWeaponInfo->PickupEffect;
	EffectColor = DropWeaponInfo->EffectColor;

	AmmoSlot = {WeaponSlot.AmmoType, DropWeaponInfo->WeaponInfo.DynamicInfo.Round, DropWeaponInfo->MaxCount};

	/* Start timer. */
	GetWorld()->GetTimerManager().SetTimer(DropTimer, this, &ThisClass::MakePickableAfterDrop, ActivationDelay, false);
}

void AP13PickingUpWeapon::ActivateParticles()
{
	if (!bDropped)
	{
		Super::ActivateParticles();
	}
}

void AP13PickingUpWeapon::BePickedUpOnCollisionBeginOverlap(AActor* Picker)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(DropTimer) || bDropped)
	{
		return;
	}
	UP13InventoryComponent* InventoryComponent = Picker->FindComponentByClass<UP13InventoryComponent>();
	if (!InventoryComponent)
	{
		return;
	}
	IP13PickupInterface* PickupInterface = Cast<IP13PickupInterface>(InventoryComponent);
	if (!PickupInterface)
	{
		return;
	}
	if (WeaponSlot.WeaponID == "None")
	{
		return;
	}

	/* Try to add weapon to the inventory. */
	PickupInterface->PickupWeaponToInventory(AmmoSlot, WeaponSlot, this);
}

void AP13PickingUpWeapon::MakePickableAfterDrop()
{
	bDropped = false;
	ActivateParticles();
}
