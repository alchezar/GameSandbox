// Copyright © 2024, IKinder

#include "P13/Public/Weapon/P13ProjectileDefault.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

AP13ProjectileDefault::AP13ProjectileDefault()
{
	PrimaryActorTick.bCanEverTick = true;

	BulletCollision = CreateDefaultSubobject<USphereComponent>("BulletCollisionSphereComponent");
	SetRootComponent(BulletCollision);
	BulletCollision->SetSphereRadius(5.f);
	BulletCollision->bReturnMaterialOnMove = true;      // Hit event return physical material
	BulletCollision->SetCanEverAffectNavigation(false); // Collision not affect navigation (P keyboard in editor)

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletStaticMesh");
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCanEverAffectNavigation(false);

	BulletTrace = CreateDefaultSubobject<UParticleSystemComponent>("BulletTraceParticleSystem");
	BulletTrace->SetupAttachment(RootComponent);

	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>("BulletProjectileMovementComponent");
	BulletMovement->UpdatedComponent = RootComponent;
	BulletMovement->InitialSpeed = 1.f;
	BulletMovement->MaxSpeed = 0.f;
	BulletMovement->bRotationFollowsVelocity = true;
	BulletMovement->bShouldBounce = true;
}

void AP13ProjectileDefault::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BulletCollision->OnComponentHit.AddDynamic(this, &ThisClass::OnBulletHitHandle);
	BulletCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBulletBeginOverlapHandle);
	BulletCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnBulletEndOverlapHandle);
}

void AP13ProjectileDefault::BeginPlay()
{
	Super::BeginPlay();
}

void AP13ProjectileDefault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP13ProjectileDefault::InitBullet(const float NewLifeSpan, const float NewInitSpeed)
{
	InitialLifeSpan = NewLifeSpan;
	BulletMovement->InitialSpeed = NewInitSpeed;
}

void AP13ProjectileDefault::OnBulletHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) 
{

}

void AP13ProjectileDefault::OnBulletBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{

}

void AP13ProjectileDefault::OnBulletEndOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex) 
{

}
