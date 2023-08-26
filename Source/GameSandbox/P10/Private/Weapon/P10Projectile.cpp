// Copyright (C) 2023, IKinder

#include "P10/Public/Weapon/P10Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "P10/Public/Util/P10Library.h"

AP10Projectile::AP10Projectile()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicatingMovement(true);

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
	SetRootComponent(CollisionComponent);
	CollisionComponent->InitSphereRadius(5.f);
	CollisionComponent->SetCollisionProfileName("Projectile");
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

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
	GetWorld()->GetTimerManager().SetTimer(ExplodeTimer, this, &ThisClass::Explode, 3.f);
}

void AP10Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP10Projectile::Explode()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFX, GetActorLocation());
	Destroy();
}

void AP10Projectile::OnCollisionHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	constexpr float Radius = 200.f;
	UP10Library::DrawDebugExplode(this, Hit, Radius);
	UP10Library::InteractWithPhysical(OtherActor, OtherComp, this);	
	UGameplayStatics::ApplyRadialDamage(this, 20, Hit.Location, Radius, nullptr, {}, GetOwner(), GetInstigator()->GetController());	
	Explode();

	/* Make some noise if on server. */
	if (HasAuthority())
	{
		MakeNoise(1.f, GetInstigator());
	}
}
