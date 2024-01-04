// Copyright © 2024, IKinder

#include "P13/Public/Actor/P13PickupActor.h"

#include "Components/SphereComponent.h"
#include "P13/Public/Component/Actor/P13InventoryComponent.h"
#include "P13/Public/Game/P13GameInstance.h"
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
	
	if (PickupType <= EP13PickupType::Weapon)
	{
		/* Add weapon to the inventory. */
		bool bWeaponPicked = false;
		if (PickupType == EP13PickupType::Weapon && PickupInterface->CheckCanTakeWeapon(WeaponSlot))
		{
			PickupInterface->TakeWeaponToInventory(WeaponSlot.WeaponID);
			bWeaponPicked = true;
			OnPickupSuccess();
		}

		/* Add ammo to the inventory. */
		/* Find correct ammo info. If there was not neither weapon no ammo type - slot will be empty. */
		const FP13AmmoSlot CorrectAmmoSlot = PickupType == EP13PickupType::Ammo ? AmmoSlot : MakeAmmoSlotFromWeaponID(WeaponSlot.WeaponID, bWeaponPicked);

		/* If we are there means CheckCanTakeWeapon == false. */
		if (PickupType <= EP13PickupType::Weapon && PickupInterface->CheckCanTakeAmmo(CorrectAmmoSlot))
		{
			PickupInterface->TakeAmmoToInventory(CorrectAmmoSlot);
			OnPickupSuccess();
		}
	}
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

FP13AmmoSlot AP13PickupActor::MakeAmmoSlotFromWeaponID(const FName WeaponID, const bool bEmptySlot) const
{
	const UP13GameInstance* GameInstance = GetWorld()->GetGameInstance<UP13GameInstance>();
	check(GameInstance)
	
	const FP13WeaponInfo* DefaultWeaponInfo = GameInstance->GetWeaponInfoByID(WeaponID);
	check(DefaultWeaponInfo)

	// const int32 Count = bEmptySlot ? 0 : DefaultWeaponInfo->MaxRound;
	const int32 Count = bEmptySlot ? 0 : AmmoSlot.Count;
	const int32 MaxCount = AmmoSlot.MaxCount;
	return {DefaultWeaponInfo->AmmoType, Count, MaxCount};
}
