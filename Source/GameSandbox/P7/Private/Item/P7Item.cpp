// Copyright (C) 2023, IKinder

#include "P7/Public/Item/P7Item.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "P7/Public/Player/P7Character.h"

AP7Item::AP7Item()
{
	PrimaryActorTick.bCanEverTick = true;
	

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMeshComponent");
	SetRootComponent(ItemMesh);
	ItemMesh->SetCollisionProfileName("NoCollision");
	
	SphereTrigger = CreateDefaultSubobject<USphereComponent>("SphereTrigger");
	SphereTrigger->SetupAttachment(RootComponent);
	SphereTrigger->SetSphereRadius(64.f);
	SphereTrigger->SetRelativeLocation(FVector(0.f, 0.f, 45.f));

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>("ItemNiagaraEffect");
	ItemEffect->SetupAttachment(RootComponent);
}

void AP7Item::BeginPlay()
{
	Super::BeginPlay();
	check(SphereTrigger);
	SphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap);
	SphereTrigger->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);
}

void AP7Item::SpawnPickupFX()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PickupEffect, GetActorLocation());
	}
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
	}
}

void AP7Item::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ItemState == EItemState::EIS_Hovering)
	{
		Oscillation(DeltaTime);
	}
}

void AP7Item::Oscillation(const float DeltaTime)
{
	RunningTime += DeltaTime;
	AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
}

void AP7Item::SetItemState(const EItemState NewState)
{
	ItemState = NewState;
}

float AP7Item::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AP7Item::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AP7Item::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IP7PickupInterface* PickupInterface = Cast<IP7PickupInterface>(OtherActor))
	{
		PickupInterface->SetOverlappingItem(this);
	}
}

void AP7Item::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IP7PickupInterface* PickupInterface = Cast<IP7PickupInterface>(OtherActor))
	{
		PickupInterface->SetOverlappingItem(nullptr);
	}
}