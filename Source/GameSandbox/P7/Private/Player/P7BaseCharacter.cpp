// Copyright (C) 2023, IKinder

#include "P7/Public/Player/P7BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P7/Public/AnimNotify/P7AttackEndNotify.h"
#include "P7/Public/AnimNotify/P7BeamTurningNotify.h"
#include "P7/Public/AnimNotify/P7BeltSnappingNotify.h"
#include "P7/Public/Component/P7AttributeComponent.h"
#include "P7/Public/Item/Weapon/P7Weapon.h"

AP7BaseCharacter::AP7BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	Attributes = CreateDefaultSubobject<UP7AttributeComponent>("AttributesComponent");
}

void AP7BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(Attributes)
	InitAnimNotifies();
	Attributes->ResetHealth();
}

void AP7BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP7BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AP7BaseCharacter::Jump()
{
	Super::Jump();
	if (!GetCharacterMovement()->IsFalling() || bDoubleJump) return;
	bDoubleJump = true;
	LaunchCharacter(FVector(0.0, 0.0, 700.0) + GetVelocity(), true, true);
}

void AP7BaseCharacter::Landed(const FHitResult& Hit)
{
	bDoubleJump = false;
	Super::Landed(Hit);
}

void AP7BaseCharacter::GetHit(const FVector& HitterLocation)
{
	if (!Attributes->GetIsAlive())
	{
		Die();
		return;
	}
	if (!bBlocking)
	{
		DirectionalHitReact(HitterLocation);
		return;
	}	
	if (BlockingHitMontage) /* Now its blocking */
	{
		PlayAnimMontage(BlockingHitMontage);
	}
}

bool AP7BaseCharacter::GetIsAttaching()
{
	return false;
}

float AP7BaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bBlocking) return 0.f;
	Attributes->ReceiveDamage(DamageAmount);
	return DamageAmount;
}

float AP7BaseCharacter::GetMovementDirectionAngle()
{
	if (FMath::IsNearlyZero(GetVelocity().Size2D())) return 0.f;
	return AngleBetweenVectors(GetActorForwardVector(), GetVelocity().GetSafeNormal());
}

void AP7BaseCharacter::SetAnimSection(const int32 StartSection)
{
	Section = StartSection;
}

void AP7BaseCharacter::SetIsBlocked(const bool bBlock)
{
	bBlocking = bBlock;
}

void AP7BaseCharacter::Attack()
{
	if (!CanAttack()) return;
	PlayMontageSection(AttackMontage);
	EquippedWeapon->OnAttackStartHandle();
}

void AP7BaseCharacter::Die()
{
	/* Ragdoll */
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->MovementState.bCanJump = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	SetLifeSpan(5.f);
}

bool AP7BaseCharacter::CanAttack()
{
	return EquippedWeapon != nullptr;
}

void AP7BaseCharacter::OnAttackEndHandle(USkeletalMeshComponent* MeshComp)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->OnAttackEndHandle();
	}

	GetWorld()->GetTimerManager().ClearTimer(ComboTimer);
	FTimerDelegate ComboDelegate;
	ComboDelegate.BindLambda([&]()
	{
		SetAnimSection(0);
	});
	GetWorld()->GetTimerManager().SetTimer(ComboTimer, ComboDelegate, 5.f, false);
}

void AP7BaseCharacter::OnBeamTurningHandle(USkeletalMeshComponent* MeshComp) {}

void AP7BaseCharacter::OnBeltSnappingHandle(USkeletalMeshComponent* MeshComp) {}

void AP7BaseCharacter::OnHitReactEndHandle(USkeletalMeshComponent* MeshComp) {}

void AP7BaseCharacter::InitAnimNotifies()
{
	TArray<FAnimNotifyEvent> AnimNotifies = AttackMontage->Notifies;
	for (const FAnimNotifyEvent& AnimNotify : AnimNotifies)
	{
		if (UP7AttackEndNotify* AttackEndNotify = Cast<UP7AttackEndNotify>(AnimNotify.Notify))
		{
			AttackEndNotify->OnAnimEnd.AddUObject(this, &ThisClass::OnAttackEndHandle);
		}
	}
	AnimNotifies.Empty();
	AnimNotifies = EquipMontage->Notifies;
	for (const FAnimNotifyEvent& AnimNotify : AnimNotifies)
	{
		if (UP7BeamTurningNotify* AttackEndNotify = Cast<UP7BeamTurningNotify>(AnimNotify.Notify))
		{
			AttackEndNotify->OnBeamTurning.AddUObject(this, &ThisClass::OnBeamTurningHandle);
		}
		if (UP7BeltSnappingNotify* AttackEndNotify = Cast<UP7BeltSnappingNotify>(AnimNotify.Notify))
		{
			AttackEndNotify->OnBeltSnapping.AddUObject(this, &ThisClass::OnBeltSnappingHandle);
		}
		if (UP7AttackEndNotify* AttackEndNotify = Cast<UP7AttackEndNotify>(AnimNotify.Notify))
		{
			AttackEndNotify->OnAnimEnd.AddUObject(this, &ThisClass::OnAttackEndHandle);
		}
	}
	AnimNotifies.Empty();
	AnimNotifies = HitReactMontage->Notifies;
	for(const FAnimNotifyEvent& AnimNotify : AnimNotifies)
	{
		if (UP7AttackEndNotify* AnimEndNotify = Cast<UP7AttackEndNotify>(AnimNotify.Notify))
		{
			AnimEndNotify->OnAnimEnd.AddUObject(this, &ThisClass::OnHitReactEndHandle);
		}
	}
}

void AP7BaseCharacter::PlayMontageSection(UAnimMontage* Montage)
{
	if (!Montage) return;
	const int32 SectionsNum = Montage->GetNumSections();
	const FName SectionName = SectionsNum > 1 ? Montage->GetSectionName(Section++ % SectionsNum) : NAME_None;
	PlayAnimMontage(Montage, 1.f, SectionName);
}

void AP7BaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	PlayAnimMontage(HitReactMontage, 1.f, SectionName);
}

void AP7BaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	FVector ToHit = (ImpactPoint - GetActorLocation()).GetSafeNormal();
	ToHit.Z = Forward.Z;
	const double Angle = AngleBetweenVectors(Forward, ToHit);

	FName SectionName = FName("FromBack");
	if		(Angle >=  -45.f && Angle <   45.f) SectionName = FName("FromFront");
	else if (Angle >= -135.f && Angle <  -45.f) SectionName = FName("FromLeft");
	else if (Angle >=   45.f && Angle <  135.f) SectionName = FName("FromRight");
	PlayHitReactMontage(SectionName);
}

double AP7BaseCharacter::AngleBetweenVectors(const FVector& Vector1, const FVector& Vector2) const
{
	const double Sign = FMath::Sign(FVector::CrossProduct(Vector1, Vector2).Z);
	return Sign * FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Vector1, Vector2)));
}
