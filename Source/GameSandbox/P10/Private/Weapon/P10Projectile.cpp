// Copyright (C) 2023, IKinder

#include "P10/Public/Weapon/P10Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AP10Projectile::AP10Projectile()
{
	PrimaryActorTick.bCanEverTick = true;

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
	if (!OtherActor || OtherActor == this || !OtherComp || !OtherComp->IsSimulatingPhysics()) return;

	/* Add impulse to hit component. */
	const float RandomIntensity = FMath::RandRange(200.f, 500.f);
	OtherComp->AddImpulseAtLocation(GetVelocity() * RandomIntensity, GetActorLocation());

	/* Make hit component smaller after each bounce. */
	const FVector Scale = OtherComp->GetComponentScale() * 0.8f;
	if (Scale.GetMin() < 0.5f)
	{
		OtherActor->Destroy();
	}
	Scale.GetMin() < 0.5f ? OtherActor->Destroy() : OtherActor->SetActorScale3D(Scale);

	/* Set random color on hit component. */
	if (UMaterialInstanceDynamic* DynamicMaterialInstance = OtherComp->CreateDynamicMaterialInstance(0))
	{
		const FLinearColor NewColor = FLinearColor::MakeRandomColor();
		DynamicMaterialInstance->SetVectorParameterValue("Color", NewColor);
	}
	Explode();
}
