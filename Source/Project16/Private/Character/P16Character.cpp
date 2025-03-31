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
	const AP16PlayerState* State = GetPlayerState<AP16PlayerState>();
	return State ? State->GetPlayerLevel() : 1;
}

inline int32 AP16Character::GetXP_Implementation() const
{
	const AP16PlayerState* State = GetPlayerState<AP16PlayerState>();
	return State ? State->GetXP() : 0;
}

int32 AP16Character::GetLevelFor_Implementation(const int32 XP) const
{
	const AP16PlayerState* State = GetPlayerState<AP16PlayerState>();
	return State ? State->LevelUpInfos->FindLevelForXP(XP) : 0;
}

int32 AP16Character::GetAttributePointsReward_Implementation(const int32 Level) const
{
	const AP16PlayerState* State = GetPlayerState<AP16PlayerState>();
	return State ? State->LevelUpInfos->LevelUpInfos[Level].AttributePointAward : 0;
}

int32 AP16Character::GetSpellPointsReward_Implementation(const int32 Level) const
{
	const AP16PlayerState* State = GetPlayerState<AP16PlayerState>();
	return State ? State->LevelUpInfos->LevelUpInfos[Level].SpellPointAward : 0;
}

void AP16Character::AddToXP_Implementation(const int32 XP)
{
	AP16PlayerState* State = GetPlayerState<AP16PlayerState>();
	EARLY_RETURN_IF(!State)

	State->AddXP(XP);
}

void AP16Character::AddToLevel_Implementation(const int32 Level)
{
	AP16PlayerState* State = GetPlayerState<AP16PlayerState>();
	EARLY_RETURN_IF(!State)

	State->AddLevel(Level);
}

void AP16Character::AddAttributePoints_Implementation(const int32 InAttributePoints)
{
	// TODO: Add attribute points to the player state.
}

void AP16Character::AddSpellPoints_Implementation(const int32 InSpellPoints)
{
	// TODO: Add spell points to the player state.
}

void AP16Character::LevelUp_Implementation()
{
	Multicast_LevelUpParticles();
}

void AP16Character::InitAbilityActorInfo()
{
	AP16PlayerState* State = GetPlayerState<AP16PlayerState>();
	EARLY_RETURN_IF(!State)

	AbilitySystemComponent = State->GetAbilitySystemComponent();
	AttributeSet           = State->GetAttributeSet();

	AbilitySystemComponent->InitAbilityActorInfo(State, this);
	AbilitySystemComponent->SetOwnerActor(GetController());

	APlayerController* PlayerController = GetController<APlayerController>();
	EARLY_RETURN_IF(!PlayerController)
	AP16HUD* HUD = PlayerController->GetHUD<AP16HUD>();
	EARLY_RETURN_IF(!HUD)
	HUD->InitOverlay({PlayerController, State, AbilitySystemComponent, AttributeSet});

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
