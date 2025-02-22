// Copyright (C) 2023, IKinder

#include "Interact/P10Objective.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/P10Character.h"

AP10Objective::AP10Objective()
{
	PrimaryActorTick.bCanEverTick = true;
	// SetReplicates(true);
	bReplicates = true;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(MeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AP10Objective::BeginPlay()
{
	Super::BeginPlay();
	check(PickupEffect)
	check(PickupSound)
	
	PlayEffect();
}

void AP10Objective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP10Objective::PlayEffect()
{
	NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffect, GetActorLocation());
	if (!NiagaraComponent) return;
	
	FTimerHandle NiagaraTimer;
	FTimerDelegate NiagaraDelegate;
	NiagaraDelegate.BindLambda([&]()
	{
		NiagaraComponent->Deactivate();
	});
	GetWorld()->GetTimerManager().SetTimer(NiagaraTimer, NiagaraDelegate, 2.f, false);
}

void AP10Objective::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffect();

	UGameplayStatics::SpawnSound2D(this, PickupSound);
	if (HasAuthority())
	{
		AP10Character* Character = Cast<AP10Character>(OtherActor);
		if (!Character) return;
	
		Character->SetCarryingObjective(true);

		Destroy();
	}
}

