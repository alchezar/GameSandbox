// Copyright (C) 2023, IKinder

#include "P7/Public/Item/Weapon/P7Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "P7/Public/Interface/P7HitInterface.h"

AP7Weapon::AP7Weapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponBox = CreateDefaultSubobject<UBoxComponent>("WeaponBoxComponent");
	WeaponBox->SetupAttachment(RootComponent);
	WeaponBox->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
	WeaponBox->SetBoxExtent(FVector(2.5f, 1.5f, 42.f));
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	TraceStart = CreateDefaultSubobject<USceneComponent>("TraceStartPoint");
	TraceStart->SetupAttachment(RootComponent);
	TraceStart->SetRelativeLocation(FVector(0.f, 0.f, 20.f));

	TraceEnd = CreateDefaultSubobject<USceneComponent>("TraceEndPoint");
	TraceEnd->SetupAttachment(RootComponent);
	TraceEnd->SetRelativeLocation(FVector(0.f, 0.f, 101.f));
}

void AP7Weapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnWeaponBeginOverlap);
}

void AP7Weapon::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP7Weapon::Equip(USceneComponent* InParent, const FName SocketName, const FSnapOffset& Offset)
{
	SetItemState(EItemState::EIS_Equipped);
	SphereTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);

	AttachToSocket(InParent, SocketName, Offset);
}

void AP7Weapon::AttachToSocket(USceneComponent* InParent, const FName SocketName, const FSnapOffset& Offset)
{
	const FAttachmentTransformRules AllRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, AllRules, SocketName);
	ItemMesh->AddRelativeLocation(Offset.LocationOffset);
	ItemMesh->AddRelativeRotation(Offset.RotationOffset);
}

void AP7Weapon::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if (OtherActor == GetOwner()) return;
	// DrawDebugPoint(GetWorld(), SweepResult.ImpactPoint, 10.f, FColor::Red, false, 10.f);
	// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Hello")));

	const FVector Start = TraceStart->GetComponentLocation();
	const FVector End = TraceStart->GetComponentLocation();
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		Start,
		End,
		FVector(5.f),
		TraceStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		IgnoredActors,
		EDrawDebugTrace::ForDuration,
		BoxHit,
		true);

	if (AActor* HitActor = BoxHit.GetActor())
	{
		IP7HitInterface* HitInterface = Cast<IP7HitInterface>(HitActor);
		if (!HitInterface) return;
		HitInterface->GetHit(BoxHit.ImpactPoint);
	}
}

void AP7Weapon::SetWeaponCollision(const ECollisionEnabled::Type CollisionType)
{
	WeaponBox->SetCollisionEnabled(CollisionType);
}
