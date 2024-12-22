// Copyright (C) 2023, IKinder

#include "Item/Weapon/P7Weapon.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "Field/FieldSystemAsset.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"
#include "Interface/P7HitInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/P7Character.h"

AP7Weapon::AP7Weapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponBox = CreateDefaultSubobject<UBoxComponent>("WeaponBoxComponent");
	WeaponBox->SetupAttachment(RootComponent);
	WeaponBox->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
	WeaponBox->SetBoxExtent(FVector(2.5f, 1.5f, 42.f));
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	FieldSystem = CreateDefaultSubobject<UFieldSystemComponent>("FieldSystem");
	RadialVector = CreateDefaultSubobject<URadialVector>("RadialVector");
	RadialFalloff = CreateDefaultSubobject<URadialFalloff>("RadialFalloff");
	MetaDataFilter = CreateDefaultSubobject<UFieldSystemMetaDataFilter>("MetaDataFilter");
}

void AP7Weapon::BeginPlay()
{
	Super::BeginPlay();
}

void AP7Weapon::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	HitTrace();
}

void AP7Weapon::SwitchWeapon(const bool bOn) {}

void AP7Weapon::SwitchWeaponHard(const bool bOn) {}

void AP7Weapon::Equip(USceneComponent* InParent, const FName SocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetItemState(EItemState::EIS_Equipped);
	SphereTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttachToBelt(InParent, SocketName);
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	EffectActivation(false);
}

void AP7Weapon::Unequip()
{
	SetItemState(EItemState::EIS_Hovering);
	SphereTrigger->SetCollisionResponseToAllChannels(ECR_Overlap);
	FVector NewLocation = GetOwner()->GetActorLocation();
	NewLocation.Z = 100.f;
	SetActorLocation(NewLocation);
	SetActorRotation(FRotator::ZeroRotator);
	EffectActivation(true);
}

void AP7Weapon::AttachToHand(USceneComponent* InParent, const FName SocketName)
{
	AttachToSocket(InParent, SocketName, HandSnapOffset);
}

void AP7Weapon::AttachToBelt(USceneComponent* InParent, const FName SocketName)
{
	AttachToSocket(InParent, SocketName, BeltSnapOffset);
}

void AP7Weapon::AttachToSocket(USceneComponent* InParent, const FName SocketName, const FSnapOffset& Offset) const
{
	const FAttachmentTransformRules AllRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, AllRules, SocketName);
	ItemMesh->AddRelativeLocation(Offset.LocationOffset);
	ItemMesh->AddRelativeRotation(Offset.RotationOffset);
}

void AP7Weapon::EffectActivation(const bool bActive) const
{
	if (!ItemEffect) return;
	bActive ? ItemEffect->Activate() : ItemEffect->Deactivate();
}

void AP7Weapon::OnAttackStartHandle() {}

void AP7Weapon::OnAttackEndHandle()
{
	SetLastTickLocation(FVector::ZeroVector);
	bAlreadyHit = false;
}

void AP7Weapon::SetWeaponCollision(const ECollisionEnabled::Type CollisionType)
{
	WeaponBox->SetCollisionEnabled(CollisionType);
}

void AP7Weapon::SetLastTickLocation(const FVector& LastLocation)
{
	LastTickLocation = LastLocation;
}

void AP7Weapon::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

void AP7Weapon::OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}

void AP7Weapon::SplashEffect(const FHitResult& HitResult) {}

void AP7Weapon::CreateFields(const FVector& FieldLocation)
{
	RadialVector->SetRadialVector(FieldMagnitude, FieldLocation);
	RadialFalloff->SetRadialFalloff(FieldMagnitude, 0.8f, 1.f, 0.f, 200.f, FieldLocation, EFieldFalloffType::Field_FallOff_None);
	MetaDataFilter->ObjectType = EFieldObjectType::Field_Object_Destruction;

	FieldSystem->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, RadialFalloff);
	FieldSystem->ApplyPhysicsField(true, EFieldPhysicsType::Field_LinearForce, MetaDataFilter, RadialVector);

	DrawDebugCapsule(GetWorld(), FieldLocation, 5.f, 10.f, FRotator::ZeroRotator.Quaternion(), FColor::Red, false, 5.f);	
}

void AP7Weapon::HitTrace()
{
	if (bAlreadyHit || !GetOwner()) return;
	OwnerChar = OwnerChar ? OwnerChar : Cast<AP7BaseCharacter>(GetOwner());
	if (!OwnerChar || !OwnerChar->GetIsAttaching()) return;
	
	FHitResult HitResult;
	SweepCapsule(HitResult);

	if (AActor* HitActor = HitResult.GetActor())
	{
		if (GetOwner()->ActorHasTag("Enemy") && HitActor->ActorHasTag("Enemy")) return;
		
		bAlreadyHit = true;
		HitResult.GetActor()->TakeDamage(Damage, FDamageEvent(), GetInstigator()->GetController(), this);
		// UGameplayStatics::ApplyDamage(HitResult.GetActor(), Damage, GetInstigator()->GetController(), this, nullptr);

		if (IP7HitInterface* HitInterface = Cast<IP7HitInterface>(HitActor))
		{
			HitInterface->GetHit(OwnerChar->GetActorLocation());
		}
		if (WeaponSound.Hit)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponSound.Hit, HitResult.ImpactPoint);
		}
		SplashEffect(HitResult);
		CreateFields(HitResult.ImpactPoint);
	}
	
}

void AP7Weapon::SweepCapsule(FHitResult& HitResult)
{
	const FVector CurrentTickLocation = WeaponBox->GetComponentLocation();	
	const FVector StartLocation = LastTickLocation == FVector::ZeroVector ? CurrentTickLocation : LastTickLocation;
	const FQuat   Rotation = GetActorRotation().Quaternion();
	
	FCollisionShape SweepShape;
	SweepShape.SetCapsule(2.f, 44.f);;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	
	GetWorld()->SweepSingleByChannel(
		HitResult, StartLocation, CurrentTickLocation, Rotation, ECC_Visibility, SweepShape, QueryParams);
	
	SetLastTickLocation(CurrentTickLocation);
}

