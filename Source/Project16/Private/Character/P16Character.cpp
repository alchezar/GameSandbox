// Copyright © 2025, Ivan Kinder

#include "Character/P16Character.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "Project16.h"
#include "AbilitySystem/Data/P16LevelUpInfoDataAsset.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/P16PlayerState.h"
#include "UI/HUD/P16HUD.h"

AP16Character::AP16Character()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate              = FRotator {0.f, 400.f, 0.f};
	GetCharacterMovement()->bConstrainToPlane         = true;
	GetCharacterMovement()->bSnapToPlaneAtStart       = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator {-45.f, 0.f, 0.f});
	SpringArm->TargetArmLength         = 750.0f;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bEnableCameraLag        = true;
	SpringArm->bInheritPitch           = false;
	SpringArm->bInheritYaw             = false;
	SpringArm->bInheritRoll            = false;

	Camera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	Camera->SetupAttachment(SpringArm.Get());
	Camera->bUsePawnControlRotation = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->SetAutoActivate(false);

	CharacterClass = EP16CharacterClass::Elemental;
}

void AP16Character::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// All child meshes that are skinned should use the leader mesh animation blueprint.
	USkeletalMeshComponent* BodyMesh = GetMesh();
	EARLY_RETURN_IF(!BodyMesh)
	for (const TObjectPtr<USceneComponent>& Child : BodyMesh->GetAttachChildren())
	{
		USkinnedMeshComponent* SkinnedChild = Cast<USkinnedMeshComponent>(Child);
		CONTINUE_IF(!SkinnedChild)
		SkinnedChild->SetLeaderPoseComponent(BodyMesh);
	}
}

void AP16Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Cache player state right after possessing and before this class logic.
	OwnPlayerState = GetPlayerState<AP16PlayerState>();
	check(OwnPlayerState)

	// Server side.
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AP16Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Client side.
	InitAbilityActorInfo();
}

void AP16Character::BeginPlay()
{
	Super::BeginPlay();
}

void AP16Character::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAbilitySystemComponent* AP16Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

int32 AP16Character::GetPlayerLevel_Implementation()
{
	return OwnPlayerState->GetPlayerLevel();
}

inline int32 AP16Character::GetXP_Implementation() const
{
	return OwnPlayerState->GetXP();
}

int32 AP16Character::GetLevelFor_Implementation(const int32 XP) const
{
	return OwnPlayerState->LevelUpInfos->FindLevelForXP(XP);
}

int32 AP16Character::GetAttributePointsReward_Implementation(const int32 Level) const
{
	return OwnPlayerState->LevelUpInfos->LevelUpInfos[Level].AttributePointAward;
}

int32 AP16Character::GetSpellPointsReward_Implementation(const int32 Level) const
{
	return OwnPlayerState->LevelUpInfos->LevelUpInfos[Level].SpellPointAward;
}

int32 AP16Character::GetAttributePoints_Implementation() const
{
	return OwnPlayerState->GetAttributePoints();
}

int32 AP16Character::GetSpellPoints_Implementation() const
{
	return OwnPlayerState->GetSpellPoints();
}

void AP16Character::AddToXP_Implementation(const int32 XP)
{
	OwnPlayerState->AddXP(XP);
}

void AP16Character::AddToLevel_Implementation(const int32 Level)
{
	OwnPlayerState->AddLevel(Level);
}

void AP16Character::AddAttributePoints_Implementation(const int32 InAttributePoints)
{
	OwnPlayerState->AddAttributePoints(InAttributePoints);
}

void AP16Character::AddSpellPoints_Implementation(const int32 InSpellPoints)
{
	OwnPlayerState->AddSpellPoints(InSpellPoints);
}

void AP16Character::LevelUp_Implementation()
{
	Multicast_LevelUpParticles();
}

void AP16Character::InitAbilityActorInfo()
{
	AbilitySystemComponent = OwnPlayerState->GetAbilitySystemComponent();
	AttributeSet           = OwnPlayerState->GetAttributeSet();

	AbilitySystemComponent->InitAbilityActorInfo(OwnPlayerState, this);
	AbilitySystemComponent->SetOwnerActor(GetController());

	APlayerController* PlayerController = GetController<APlayerController>();
	EARLY_RETURN_IF(!PlayerController)
	AP16HUD* HUD = PlayerController->GetHUD<AP16HUD>();
	EARLY_RETURN_IF(!HUD)
	HUD->InitOverlay({PlayerController, OwnPlayerState, AbilitySystemComponent, AttributeSet});

	Super::InitAbilityActorInfo();
}

UAttributeSet* AP16Character::GetAttributeSet() const
{
	return AttributeSet.Get();
}

void AP16Character::Multicast_LevelUpParticles_Implementation() const
{
	check(LevelUpNiagaraComponent)
	LevelUpNiagaraComponent->Activate(true);
}
