// Copyright © 2025, Ivan Kinder

#include "Character/P16CharacterBase.h"

#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Root/Public/Singleton/GSGameplayTagsSingleton.h"

AP16CharacterBase::AP16CharacterBase()
{
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSkeletalMeshComponent");
	Weapon->SetupAttachment(GetMesh(), HandSocketName);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (CombatSocketNameMap.IsEmpty())
	{
		const auto SocketTags = FGSGameplayTagsSingleton::Get().P16Tags.CombatSocket;

		CombatSocketNameMap = {
			{SocketTags.WeaponTag, "TipSocket"},
			{SocketTags.LeftHandTag, "LeftHand"},
			{SocketTags.RightHandTag, "RightHand"}};
	}
}

void AP16CharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

FVector AP16CharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag MontageTag)
{
	// Find which mesh has a socket.
	const USceneComponent* SocketComponent = Weapon;
	if (!MontageTag.MatchesTagExact(FGSGameplayTagsSingleton::Get().P16Tags.CombatSocket.WeaponTag))
	{
		SocketComponent = GetMesh();
	}

	// Get a location of that socket.
	EARLY_RETURN_VALUE_IF(!SocketComponent || !CombatSocketNameMap.Contains(MontageTag), GetActorLocation());
	return SocketComponent->GetSocketLocation(CombatSocketNameMap[MontageTag]);
}

bool AP16CharacterBase::GetIsDead_Implementation() const
{
	return bDead;
}

AActor* AP16CharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FP16TaggedMontage> AP16CharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

FP16TaggedMontage AP16CharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	FP16TaggedMontage* Result = AttackMontages.FindByPredicate([MontageTag](const FP16TaggedMontage& Each) -> bool {
		return Each.Attack.MatchesTagExact(MontageTag);
	});

	return Result ? *Result : FP16TaggedMontage{};
}

UNiagaraSystem* AP16CharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

UAnimMontage* AP16CharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage.Get();
}

void AP16CharacterBase::Die()
{
	if (Weapon)
	{
		Weapon->DetachFromComponent(FDetachmentTransformRules{EDetachmentRule::KeepWorld, true});
	}
	Multicast_Die();
}

void AP16CharacterBase::InitAbilityActorInfo()
{
	if (UP16AbilitySystemComponent* AbilitySystem = Cast<UP16AbilitySystemComponent>(AbilitySystemComponent))
	{
		AbilitySystem->OnAbilityActorInfoSet();
	}
	InitDefaultAttributes();
}

void AP16CharacterBase::InitDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes);
	ApplyEffectToSelf(DefaultSecondaryAttributes);
	ApplyEffectToSelf(DefaultVitalAttributes);
}

void AP16CharacterBase::Multicast_Die_Implementation()
{
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	if (Weapon)
	{
		Weapon->SetSimulatePhysics(true);
		Weapon->SetEnableGravity(true);
		Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	bDead = true;
}

void AP16CharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& InGameplayEffect, const float InLevel) const
{
	check(AbilitySystemComponent)
	EARLY_RETURN_IF(!InGameplayEffect)

	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(this);
	const FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(InGameplayEffect, InLevel, Context);
	AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), AbilitySystemComponent.Get());
}

void AP16CharacterBase::AddCharacterAbilities() const
{
	EARLY_RETURN_IF(!HasAuthority())

	UP16AbilitySystemComponent* AbilitySystem = Cast<UP16AbilitySystemComponent>(AbilitySystemComponent);
	EARLY_RETURN_IF(!AbilitySystem)

	AbilitySystem->AddCharacterAbilities(StartupAbilities);
}

void AP16CharacterBase::Dissolve()
{
	TArray<UMaterialInstanceDynamic*> DynamicMaterials = {};
	if (BodyDissolveMaterial)
	{
		UMaterialInstanceDynamic* DynamicBodyDissolveMaterial = UMaterialInstanceDynamic::Create(BodyDissolveMaterial.Get(), this);
		GetMesh()->SetMaterial(0, DynamicBodyDissolveMaterial);
		DynamicMaterials.Add(DynamicBodyDissolveMaterial);
	}
	if (WeaponDissolveMaterial)
	{
		UMaterialInstanceDynamic* DynamicWeaponDissolveMaterial = UMaterialInstanceDynamic::Create(WeaponDissolveMaterial.Get(), this);
		Weapon->SetMaterial(0, DynamicWeaponDissolveMaterial);
		DynamicMaterials.Add(DynamicWeaponDissolveMaterial);
	}
	EARLY_RETURN_IF(DynamicMaterials.IsEmpty());
	StartDissolveTimeline(DynamicMaterials);
}
