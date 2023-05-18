// Copyright (C) 2023, IKinder

#include "TG_Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "P4/Player/TG_BaseCharacter.h"
#include "P4/Weapon/TG_Gun.h"
#include "Engine/DamageEvents.h"

ATG_Projectile::ATG_Projectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	// CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComp->OnComponentHit.AddDynamic(this, &ATG_Projectile::OnHit);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	SetRootComponent(CollisionComp);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;

	InitialLifeSpan = 3.0f;
}

void ATG_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// if (OtherActor && OtherComp && OtherActor != this && OtherComp->IsSimulatingPhysics())
	// {
	// 	OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	// }
	if (AActor* HitActor = Hit.GetActor())
	{
		FPointDamageEvent DamageEvent;
		DamageEvent.HitInfo = Hit;
		HitActor->TakeDamage(ProjectileOwner->GetWeaponDamage(), DamageEvent, nullptr, this);
	}
	// if (ATG_BaseCharacter* Character = Cast<ATG_BaseCharacter>(OtherActor))
	// {		
	// 	Character->ReceiveDamage(ProjectileOwner->GetWeaponDamage());
	// }
	
	Destroy();
}

USphereComponent* ATG_Projectile::GetCollisionComp() const
{
	return CollisionComp;
}

UProjectileMovementComponent* ATG_Projectile::GetProjectileMovement() const
{
	return ProjectileMovement;
}

void ATG_Projectile::SetProjectileOwner(ATG_Gun* NewProjectileOwner)
{
	ProjectileOwner = NewProjectileOwner;
}
