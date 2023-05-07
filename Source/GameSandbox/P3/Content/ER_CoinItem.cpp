// Copyright (C) 2023, IKinder

#include "ER_CoinItem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ER_Character.h"

AER_CoinItem::AER_CoinItem()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	SphereCollider = CreateDefaultSubobject<USphereComponent>("SphereCollider");
	SphereCollider->SetupAttachment(GetRootComponent());
	SphereCollider->SetCollisionProfileName("OverlapOnlyPawn");

	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>("CoinMesh");
	CoinMesh->SetupAttachment(SphereCollider);
	CoinMesh->SetCollisionProfileName("OverlapOnlyPawn");

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>("RotatingMovement");
	RotatingMovement->RotationRate = FRotator(0.f, 180.f, 0.f);
}

void AER_CoinItem::BeginPlay()
{
	Super::BeginPlay();

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnColliderBeginOverlap);	
}

void AER_CoinItem::OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (AER_Character* Character = Cast<AER_Character>(OtherActor))
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), OverlapSound, GetActorLocation());
		Character->AddCoin();
		CoinMesh->SetVisibility(false);
	}
}

float AER_CoinItem::GetSpawnProbability()
{
	return SpawnProbability;
}
