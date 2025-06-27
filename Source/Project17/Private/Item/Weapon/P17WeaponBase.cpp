// Copyright © 2025, Ivan Kinder

#include "Item/Weapon/P17WeaponBase.h"

#include "Project17.h"
#include "Components/BoxComponent.h"

AP17WeaponBase::AP17WeaponBase()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponStaticMeshComponent");
	WeaponMesh->SetupAttachment(GetRootComponent());

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>("WeaponCollisionBoxComponent");
	WeaponCollisionBox->SetupAttachment(WeaponMesh);
	WeaponCollisionBox->SetBoxExtent(FVector {20.f});
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBeginOverlapCallback);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionEndOverlapCallback);
}

void AP17WeaponBase::ToggleCollision(const bool bEnable) const
{
	WeaponCollisionBox->SetCollisionEnabled(bEnable
		? ECollisionEnabled::QueryOnly
		: ECollisionEnabled::NoCollision);
}

void AP17WeaponBase::OnCollisionBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const APawn* OwningPawn = GetInstigator<APawn>();
	WARN_RETURN_IF(!OwningPawn,)

	const APawn* HitPawn = Cast<APawn>(OtherActor);
	RETURN_IF(!HitPawn || OwningPawn == HitPawn,)

	OnWeaponHitTarget.Execute(OtherActor);

	// TODO: Implement hit check for enemy characters.
}

void AP17WeaponBase::OnCollisionEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	const APawn* OwningPawn = GetInstigator<APawn>();
	WARN_RETURN_IF(!OwningPawn,)

	const APawn* HitPawn = Cast<APawn>(OtherActor);
	RETURN_IF(!HitPawn || OwningPawn == HitPawn,)

	OnWeaponPulledFromTarget.Execute(OtherActor);

	// TODO: Implement hit check for enemy characters.
}
