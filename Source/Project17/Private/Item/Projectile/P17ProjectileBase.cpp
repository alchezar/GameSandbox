// Copyright © 2025, Ivan Kinder

#include "Item/Projectile/P17ProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraComponent.h"
#include "Project17.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Util/P17FunctionLibrary.h"
#include "Util/P17GameplayTags.h"

AP17ProjectileBase::AP17ProjectileBase()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBoxComponent");
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("ProjectileNiagaraComponent");
	NiagaraComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 700.f;
	ProjectileMovementComponent->MaxSpeed = 900.f;
	ProjectileMovementComponent->Velocity = FVector::ForwardVector;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 4.f;
}

void AP17ProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlapCallback);
	CollisionBox->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnProjectileHitCallback);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, DamagePolicy == EP17ProjectileDamagePolicy::OnBeginOverlap ? ECR_Overlap : ECR_Block);
}

void AP17ProjectileBase::OnProjectileBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	WARN_RETURN_IF(OtherActor == GetInstigator(),)
	RETURN_IF(OverlappedActors.Contains(OtherActor),)
	OverlappedActors.Add(OtherActor);
	SendDamageTo(OtherActor);
}

void AP17ProjectileBase::OnProjectileHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	WARN_RETURN_IF(OtherActor == GetInstigator(),)
	BP_OnSpawnProjectileHitFX(Hit.ImpactPoint);
	SendDamageTo(OtherActor);
	Destroy();
}

void AP17ProjectileBase::SendDamageTo(AActor* TargetActor)
{
	APawn* InstigatorPawn = GetInstigator();
	WARN_RETURN_IF(!InstigatorPawn,)

	APawn* HitPawn = Cast<APawn>(TargetActor);
	RETURN_IF(!HitPawn || !UP17FunctionLibrary::IsTargetHostile(InstigatorPawn, HitPawn),)

	const bool bPlayerBlocking = UP17FunctionLibrary::NativeGetActorHasTag(HitPawn, P17::Tags::Player_Status_Blocking);
	const bool bUnblockableAttack = UP17FunctionLibrary::NativeGetActorHasTag(InstigatorPawn, P17::Tags::Enemy_Status_Unblockable);

	const bool bValidBlock = UP17FunctionLibrary::IsValidBlock(InstigatorPawn, HitPawn)
		&& bPlayerBlocking
		&& !bUnblockableAttack;

	FGameplayEventData Payload {};
	Payload.Instigator = this;
	Payload.Target = HitPawn;

	if (bValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitPawn, P17::Tags::Player_Event_Hit_SuccessfulBlock, Payload);
	}
	else
	{
		HandleApplyProjectileDamage(HitPawn, Payload);
	}
}

void AP17ProjectileBase::HandleApplyProjectileDamage(APawn* InHitPawn, const FGameplayEventData& InPayload) const
{
	WARN_RETURN_IF(!DamageEffectSpecHandle.IsValid(),)

	const bool bApplied = UP17FunctionLibrary::ApplyGameplayEffectSpecHandle(GetInstigator(), InHitPawn, DamageEffectSpecHandle);
	RETURN_IF(!bApplied,)

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(InHitPawn, P17::Tags::Shared_Event_Hit_React, InPayload);
}
