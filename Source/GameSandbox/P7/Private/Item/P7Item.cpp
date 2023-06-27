// Copyright (C) 2023, IKinder

#include "P7/Public/Item/P7Item.h"
#include "Components/SphereComponent.h"
#include "P7/Public/Player/P7Character.h"

AP7Item::AP7Item()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereTrigger = CreateDefaultSubobject<USphereComponent>("SphereTrigger");
	SetRootComponent(SphereTrigger);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMeshComponent");
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->SetCollisionProfileName("NoCollision");
}

void AP7Item::BeginPlay()
{
	Super::BeginPlay();

	SphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap);
	SphereTrigger->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);
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
	if (AP7Character* Character = Cast<AP7Character>(OtherActor))
	{
		Character->SetOverlappingItem(this);
	}
}

void AP7Item::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AP7Character* Character = Cast<AP7Character>(OtherActor))
	{
		Character->SetOverlappingItem(nullptr);
	}
}