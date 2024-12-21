// Copyright (C) 2023, IKinder

#include "Pickup/STU_BasePickup.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ASTU_BasePickup::ASTU_BasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->InitSphereRadius(50.f);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	SetRootComponent(SphereComponent);
}

void ASTU_BasePickup::BeginPlay()
{
	Super::BeginPlay();
	check(SphereComponent);
	
	GenerateRotationYaw();
}

void ASTU_BasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.0, RotationYaw, 0.0));
}

bool ASTU_BasePickup::CouldBeTaken() const 
{
	return !GetWorldTimerManager().IsTimerActive(RespawnTimer);
}

void ASTU_BasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn) return;
	
	if (TryToGivePickup(Pawn))
	{
		PickupWasTaken();
	}
}

bool ASTU_BasePickup::TryToGivePickup(APawn* PlayerPawn)
{
	return false;
}

void ASTU_BasePickup::PickupWasTaken()
{
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetRootComponent()->SetVisibility(false, true);

	GetWorldTimerManager().SetTimer(RespawnTimer, this, &ASTU_BasePickup::Respawn, RespawnTime);	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
}

void ASTU_BasePickup::Respawn()
{
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	GetRootComponent()->SetVisibility(true, true);
	GenerateRotationYaw();
}

void ASTU_BasePickup::GenerateRotationYaw()
{
	const double Direction = FMath::RandBool() ? 1.0 : -1.0;
	RotationYaw = FMath::Abs(RotationYaw) * Direction;
}
