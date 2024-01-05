// Copyright © 2024, IKinder

#include "P13/Public/Actor/P13PickupActor.h"

#include "Components/SphereComponent.h"
#include "P13/Public/Component/Actor/P13InventoryComponent.h"
#include "P13/Public/Intearface/P13PickupInterface.h"

AP13PickupActor::AP13PickupActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CreateComponents();
}

void AP13PickupActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCollisionBeginOverlapHandle);
}

void AP13PickupActor::BeginPlay()
{
	Super::BeginPlay();

	Collision->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AP13PickupActor::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP13PickupActor::OnCollisionBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UP13InventoryComponent* InventoryComponent = OtherActor->FindComponentByClass<UP13InventoryComponent>();
	if (!InventoryComponent)
	{
		return;
	}
	IP13PickupInterface* PickupInterface = Cast<IP13PickupInterface>(InventoryComponent);
	if (!PickupInterface)
	{
		return;
	}

	/* Add ammo to the inventory. */
	if (PickupType == EP13PickupType::Ammo)
	{
		if (!PickupInterface->TryTakeAmmoToInventory(AmmoSlot))
		{
			return;
		}
	}

	/* Add weapon to the inventory. */
	else if (PickupType == EP13PickupType::Weapon)
	{
		const bool bWeaponTaken = PickupInterface->TryTakeWeaponToInventory(WeaponSlot);
		/* When we successfully took the weapon, but there is no slot for its ammo in the inventory -
		 * try to add an empty one, by resetting count in ammo slot. */
		AmmoSlot.Count = bWeaponTaken ? 0 : AmmoSlot.Count;
		/* Or if we already have this weapon in the inventory - simply add one magazine to its ammo slot. */
		if (!PickupInterface->TryTakeAmmoToInventory(AmmoSlot) && !bWeaponTaken)
		{
			return;
		}
	}

	OnPickupSuccess();
}

void AP13PickupActor::CreateComponents()
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

void AP13PickupActor::OnPickupSuccess()
{
	Destroy();
}
