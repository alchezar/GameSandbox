// Copyright (C) 2023, IKinder

#include "ARAbilityProjectileBase.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Character.h"
#include "P6/Weapon/ARProjectileMagic.h"

UARAbilityProjectileBase::UARAbilityProjectileBase()
{}

void UARAbilityProjectileBase::StartAbility_Implementation(AActor* Instigator)
{
	Super::StartAbility_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (!Character) return;

	if (AttackMontage)
	{
		Character->PlayAnimMontage(AttackMontage);
	}

	const FVector Start = Character->GetPawnViewLocation();
	const FVector End = Start + Character->GetControlRotation().Vector() * FVector(10000.f);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);

	if (Character->HasAuthority())
	{
		FTimerHandle AttackTimer;
		FTimerDelegate AttackDelegate;
		AttackDelegate.BindUFunction(this, "SpawnProjectile", Character, bHomingProjectile, ProjectileClass, HitResult);

		GetWorld()->GetTimerManager().SetTimer(AttackTimer, AttackDelegate, 0.2f, false, 0.2f);
	}
}

void UARAbilityProjectileBase::StopAbility_Implementation(AActor* Instigator)
{
	Super::StopAbility_Implementation(Instigator);
}

void UARAbilityProjectileBase::SpawnProjectile(ACharacter* Instigator, const bool bHoming, const TSubclassOf<AActor> ClassToSpawn, const FHitResult& HitResult)
{
	const FVector HandLocation = Instigator->GetMesh()->GetSocketLocation(HandSocketName);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = Instigator;

	if (HitResult.bBlockingHit && bHoming)
	{
		ATargetPoint* Target = GetWorld()->SpawnActor<ATargetPoint>(ATargetPoint::StaticClass(), HitResult.ImpactPoint, FRotator::ZeroRotator);
		Target->SetLifeSpan(5.f);

		AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ClassToSpawn, FTransform(Instigator->GetActorRotation(), HandLocation), SpawnParams);
		if (!SpawnedProjectile) return;
		AARProjectileMagic* SpawnedHomingProjectile = Cast<AARProjectileMagic>(SpawnedProjectile);
		if (!SpawnedHomingProjectile) return;

		SpawnedHomingProjectile->AddActorToIgnore(Instigator);
		SpawnedHomingProjectile->SetTarget(Target);

		StopAbility(Instigator);
		return;
	}

	const FVector HomingLocation = Instigator->GetPawnViewLocation() + Instigator->GetControlRotation().Vector() * FVector(10000.f);
	const FVector TargetLocation = HitResult.bBlockingHit ? HitResult.ImpactPoint : HomingLocation;

	const FRotator CorrectRotation = FRotationMatrix::MakeFromX(TargetLocation - HandLocation).Rotator();
	const FTransform SpawnTransform = FTransform(CorrectRotation, HandLocation, FVector(1.f));

	GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTransform, SpawnParams);

	StopAbility(Instigator);
}
