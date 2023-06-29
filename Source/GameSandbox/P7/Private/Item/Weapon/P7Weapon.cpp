// Copyright (C) 2023, IKinder

#include "P7/Public/Item/Weapon/P7Weapon.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "P7/Public/Interface/P7HitInterface.h"
#include "P7/Public/Player/P7Character.h"

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
	// WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnWeaponBeginOverlap);
}

void AP7Weapon::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	HitTrace();
}

void AP7Weapon::Equip(USceneComponent* InParent, const FName SocketName, const FSnapOffset& Offset)
{
	SetItemState(EItemState::EIS_Equipped);
	SphereTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	SwitchRibbon(true);
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
	/*
	if (OtherActor == GetOwner()) return;
	DrawDebugPoint(GetWorld(), SweepResult.ImpactPoint, 10.f, FColor::Red, false, 10.f);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Hello")));
	
	const FVector Start = TraceStart->GetComponentLocation();
	const FVector End = TraceEnd->GetComponentLocation();
	FQuat Rotation = TraceStart->GetComponentRotation().Quaternion();
	
	FHitResult HitResult;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
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
		HitResult,
		true);
	
	if (AActor* HitActor = HitResult.GetActor())
	{
		IP7HitInterface* HitInterface = Cast<IP7HitInterface>(HitActor);
		if (!HitInterface) return;
		HitInterface->GetHit(HitResult.ImpactPoint);
	}
*/
}

void AP7Weapon::SetWeaponCollision(const ECollisionEnabled::Type CollisionType)
{
	WeaponBox->SetCollisionEnabled(CollisionType);
}

void AP7Weapon::SetLastTickLocation(const FVector& LastLocation)
{
	LastTickLocation = LastLocation;
}

void AP7Weapon::HitTrace()
{
	if (!GetOwner() || bAlreadyHit) return;
	if (!OwnerAsCharacter) OwnerAsCharacter = Cast<AP7Character>(GetOwner());
	if (OwnerAsCharacter->GetActionState() != EAS_Attacking) return;

	const bool bFirstTick = LastTickLocation == FVector::ZeroVector;
	const FVector CurrentTickLocation = WeaponBox->GetComponentLocation();	
	FHitResult HitResult;
	FCollisionShape SweepShape;
	SweepShape.SetCapsule(2.f, 44.f);

	GetWorld()->SweepSingleByChannel(
		HitResult,
		!bFirstTick ? LastTickLocation : CurrentTickLocation,
		CurrentTickLocation,
		GetActorRotation().Quaternion(),
		ECC_Visibility,
		SweepShape);

	if (AActor* HitActor = HitResult.GetActor())
	{
		IP7HitInterface* HitInterface = Cast<IP7HitInterface>(HitActor);
		if (!HitInterface) return;
		HitInterface->GetHit(OwnerAsCharacter->GetActorLocation());
		bAlreadyHit = true;
		if (WeaponSound.Hit)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponSound.Hit, HitResult.ImpactPoint);
			SplashEffect(HitResult);
		}
	}
	SetLastTickLocation(CurrentTickLocation);
}

void AP7Weapon::OnAttackStartHandle()
{
	SetWeaponCollision(ECollisionEnabled::QueryOnly);
	SwitchRibbon(true);
}

void AP7Weapon::OnAttackEndHandle()
{
	SetWeaponCollision(ECollisionEnabled::NoCollision);
	SetLastTickLocation(FVector::ZeroVector);
	SwitchRibbon(false);
	bAlreadyHit = false;
}

void AP7Weapon::SplashEffect(const FHitResult& HitResult) {}


void AP7Weapon::SwitchWeapon(const bool bOn) {}

void AP7Weapon::SwitchWeaponHard(const bool bOn) {}

void AP7Weapon::SwitchRibbon(const bool bOn) {}

