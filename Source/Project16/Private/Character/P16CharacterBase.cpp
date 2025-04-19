// Copyright © 2025, Ivan Kinder

#include "Character/P16CharacterBase.h"

#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "AbilitySystem/Debuff/P16DebuffNiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Root/Public/Singleton/GSGameplayTagsSingleton.h"

AP16CharacterBase::AP16CharacterBase()
{
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSkeletalMeshComponent");
	Weapon->SetupAttachment(GetMesh(), HandSocketName);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BurnDebuff = CreateDefaultSubobject<UP16DebuffNiagaraComponent>("DebuffNiagaraComponent");
	BurnDebuff->SetupAttachment(GetRootComponent());
	BurnDebuff->DebuffTag = FGSGameplayTagsSingleton::Get().P16Tags.Debuff.Type.BurnTag;

	StunDebuff = CreateDefaultSubobject<UP16DebuffNiagaraComponent>("StunDebuffNiagaraComponent");
	StunDebuff->SetupAttachment(GetRootComponent());
	StunDebuff->DebuffTag = FGSGameplayTagsSingleton::Get().P16Tags.Debuff.Type.StunTag;

	if (CombatSocketNameMap.IsEmpty())
	{
		const auto SocketTags = FGSGameplayTagsSingleton::Get().P16Tags.CombatSocket;

		CombatSocketNameMap = {
			{SocketTags.WeaponTag, P16::Socket::Weapon},
			{SocketTags.LeftHandTag, P16::Socket::LeftHand},
			{SocketTags.RightHandTag, P16::Socket::RightHand}
		};
	}
}

void AP16CharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AP16CharacterBase, bBurned);
	DOREPLIFETIME(AP16CharacterBase, bStunned);
	DOREPLIFETIME(AP16CharacterBase, bBeingShocked);
}

void AP16CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	BaseWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
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

bool AP16CharacterBase::GetBeingShocked_Implementation() const
{
	return bBeingShocked;
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
	FP16TaggedMontage* Result = AttackMontages.FindByPredicate([MontageTag](const FP16TaggedMontage& Each) -> bool
	{
		return Each.Attack.MatchesTagExact(MontageTag);
	});

	return Result ? *Result : FP16TaggedMontage {};
}

UNiagaraSystem* AP16CharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

int32 AP16CharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AP16CharacterBase::IncrementMinionCount_Implementation(const int32 Amount)
{
	MinionCount += Amount;
}

EP16CharacterClass AP16CharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FP16OnAbilitySystemRegisteredSignature& AP16CharacterBase::GetOnAbilitySystemRegisteredDelegate()
{
	return OnAbilitySystemRegistered;
}

FP16OnDeathSignature& AP16CharacterBase::GetOnDeathDelegate()
{
	return OnDeath;
}

USkeletalMeshComponent* AP16CharacterBase::GetWeapon_Implementation()
{
	return Weapon.Get();
}

void AP16CharacterBase::SetBeingShocked_Implementation(const bool bIsShocked)
{
	bBeingShocked = bIsShocked;

	EARLY_RETURN_IF(!AbilitySystemComponent)

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(FGSGameplayTagsSingleton::Get().P16Tags.Effect.BeingShockedTag);

	if (bBeingShocked)
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
	}
	else
	{
		AbilitySystemComponent->CancelAbilities(&TagContainer);
	}
}

UAnimMontage* AP16CharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage.Get();
}

void AP16CharacterBase::Die(const FVector& DeathImpulse)
{
	if (Weapon)
	{
		Weapon->DetachFromComponent(FDetachmentTransformRules {EDetachmentRule::KeepWorld, true});
	}
	Multicast_Die(DeathImpulse);
}

void AP16CharacterBase::InitAbilityActorInfo()
{
	OnAbilitySystemRegistered.Broadcast(AbilitySystemComponent);

	// Listen to debuff applications.
	AbilitySystemComponent->RegisterGameplayTagEvent(FGSGameplayTagsSingleton::Get().P16Tags.Debuff.Type.BurnTag)
		.AddUObject(this, &ThisClass::OnBurnTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(FGSGameplayTagsSingleton::Get().P16Tags.Debuff.Type.StunTag)
		.AddUObject(this, &ThisClass::OnStunTagChanged);

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

void AP16CharacterBase::Multicast_Die_Implementation(const FVector& DeathImpulse)
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
		Weapon->AddImpulse(DeathImpulse / 8.f, NAME_None, true);
	}

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	bDead = true;
	OnDeath.Broadcast(this);
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
	AbilitySystem->AddCharacterPassiveAbilities(StartupPassiveAbilities);
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

void AP16CharacterBase::OnBurnTagChanged(const FGameplayTag CallbackTag, const int32 NewCount)
{
	bBurned = NewCount > 0;

	if (HasAuthority())
	{
		OnRep_Burned();
	}
}

void AP16CharacterBase::OnStunTagChanged(const FGameplayTag CallbackTag, const int32 NewCount)
{
	bStunned = NewCount > 0;

	GetCharacterMovement()->MaxWalkSpeed = bStunned ? 0.f : BaseWalkSpeed;

	// Don't allow to move while stunned. As it is called on Server, `OnRep_Stunned()` will be called on Clients.
	if (HasAuthority())
	{
		OnRep_Stunned();
	}
}

void AP16CharacterBase::OnRep_Burned(const bool bOldBurned) const
{
	BurnDebuff->ToggleDebuff(bBurned);
}

void AP16CharacterBase::OnRep_Stunned(const bool bOldStunned) const
{
	UP16AbilitySystemComponent* AbilitySystem = Cast<UP16AbilitySystemComponent>(AbilitySystemComponent);
	EARLY_RETURN_IF(!AbilitySystem)
	AbilitySystem->UpdateStunStatus(bStunned);

	StunDebuff->ToggleDebuff(bStunned);
}
