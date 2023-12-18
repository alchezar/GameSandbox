// Copyright (C) 2023, IKinder

#include "P10/Public/Weapon/P10Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "HAL/IConsoleManager.h"
#include "Kismet/GameplayStatics.h"
#include "P10/Public/Util/P10Library.h"
#include "P10/Public/Weapon/P10Weapon.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

AP10Projectile::AP10Projectile()
{
	PrimaryActorTick.bCanEverTick = false;
	// SetReplicates(true);
	bReplicates = true;
	SetReplicatingMovement(true);

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
	SetRootComponent(CollisionComponent);
	CollisionComponent->InitSphereRadius(5.f);
	CollisionComponent->SetCollisionProfileName("Projectile");
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->bReturnMaterialOnMove = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMeshComponent");
	ProjectileMesh->SetupAttachment(RootComponent);
}

void AP10Projectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnCollisionHitHandle);

	FTimerHandle ExplodeTimer;
	FTimerDelegate ExplodeDelegate;
	
	ExplodeDelegate.BindLambda([&]()
	{
		const FHitResult ExplodeHit;
		Explode(ExplodeHit);
	});
	GetWorld()->GetTimerManager().SetTimer(ExplodeTimer, ExplodeDelegate, 3.f, false);
}

void AP10Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP10Projectile::Explode(const FHitResult& Hit)
{
	if (AP10Weapon* OwnerWeapon = Cast<AP10Weapon>(GetOwner()))
	{
		const EPhysicalSurface Surface = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		OwnerWeapon->PlayImpactEffect(Surface, Hit.ImpactPoint);
	}
	Destroy();
}

void AP10Projectile::OnCollisionHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	constexpr float Radius = 200.f;
	
	UP10Library::DrawDebugExplode(this, Hit.Location, Radius);
	UP10Library::InteractWithPhysical(OtherActor, OtherComp, this);	
	UGameplayStatics::ApplyRadialDamage(this, 20, Hit.Location, Radius, nullptr, {}, GetOwner(), GetInstigator()->GetController());	
	Explode(Hit);

	/* Make some noise if on server. */
	if (HasAuthority())
	{
		MakeNoise(1.f, GetInstigator());
	}
}
