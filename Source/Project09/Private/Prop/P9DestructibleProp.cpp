// Copyright (C) 2023, IKinder

#include "Prop/P9DestructibleProp.h"

#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

AP9DestructibleProp::AP9DestructibleProp()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	SetRootComponent(SceneComponent);

	DestructibleComponent = CreateDefaultSubobject<UGeometryCollectionComponent>("DestructibleGeometryCollectionComponent");
	DestructibleComponent->SetupAttachment(RootComponent);
	DestructibleComponent->SetNotifyRigidBodyCollision(true);
	DestructibleComponent->SetNotifyBreaks(true);
	DestructibleComponent->SetCollisionResponseToAllChannels(ECR_Block);

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>("BoxTriggerComponent");
	TriggerComponent->SetupAttachment(RootComponent);
	TriggerComponent->SetGenerateOverlapEvents(true);
}

void AP9DestructibleProp::BeginPlay()
{
	Super::BeginPlay();
	check(SceneComponent)
	check(DestructibleComponent)
	check(TriggerComponent)

	DestructibleComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnDestructibleHitHandle);
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTriggerBeginOverlapHandle);

	CurrentHealth = MaxHealth;
}

void AP9DestructibleProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP9DestructibleProp::OnDestructibleHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bDestroyed || !OtherComp->ComponentHasTag("Weapon")) return;
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, __FUNCTION__);

	CurrentHealth -= DefaultDamage;
	if (CurrentHealth <= 0.f)
	{
		Destruct(DefaultDamage, Hit.ImpactPoint, Hit.ImpactNormal * -1.f, DefaultImpulse);
	}
}

void AP9DestructibleProp::OnTriggerBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, __FUNCTION__);
	if (bDestroyed || !bTriggerEnabled || !OtherActor->ActorHasTag("Player")) return;

	Destruct(DefaultDamage, DestructibleComponent->GetComponentLocation(), DestructibleComponent->GetUpVector(), DefaultImpulse);
}

void AP9DestructibleProp::Destruct(const float Damage, const FVector& HitLocation, const FVector& ImpulseDirection, const float Impulse)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, __FUNCTION__);
	// if (!bDestroyed) bDestroyed = true;

	DestructibleComponent->AddImpulseAtLocation(ImpulseDirection * Impulse, HitLocation);
	DestructibleComponent->AddForce(ImpulseDirection * Impulse);
}
