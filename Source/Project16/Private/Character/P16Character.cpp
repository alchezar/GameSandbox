// Copyright © 2025, Ivan Kinder

#include "Character/P16Character.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "AbilitySystem/P16AbilitySystemLibrary.h"
#include "AbilitySystem/Data/P16LevelUpInfoDataAsset.h"
#include "Camera/CameraComponent.h"
#include "Game/P16GameMode.h"
#include "Game/P16SaveGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/P16PlayerController.h"
#include "Player/P16PlayerState.h"
#include "UI/HUD/P16HUD.h"
#include "Util/P16Log.h"

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

void AP16Character::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);

	// Respawn after death.
	FTimerHandle         DeathTimer;
	const FTimerDelegate DeathDelegate = FTimerDelegate::CreateWeakLambda(this, [this]() -> void
	{
		const AP16GameMode* GameMode = GetWorld()->GetAuthGameMode<AP16GameMode>();
		EARLY_RETURN_IF(!GameMode)

		GameMode->PlayerDied(GetController());
	});
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, DeathDelegate, RespawnTime, false);
	Camera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
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

void AP16Character::AddToLevel_Implementation(const int32 DeltaLevel)
{
	OwnPlayerState->AddLevel(DeltaLevel);

	if (UP16AbilitySystemComponent* AbilitySystem = Cast<UP16AbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AbilitySystem->UpdateAbilityStatuses(OwnPlayerState->GetPlayerLevel());
	}
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

void AP16Character::ToggleMagicCircle_Implementation(const bool bOn, UMaterialInterface* DecalMaterial)
{
	AP16PlayerController* PlayerController = Cast<AP16PlayerController>(Controller);
	EARLY_RETURN_IF(!PlayerController)

	PlayerController->ToggleMagicCircle(bOn, DecalMaterial);
	PlayerController->bShowMouseCursor = !bOn;
}

void AP16Character::SaveProgress_Implementation(const FName& CheckpointTag)
{
	const AP16GameMode* GameMode = GetWorld()->GetAuthGameMode<AP16GameMode>();
	EARLY_RETURN_IF(!GameMode)
	UP16SaveGame* SaveGame = GameMode->GetInGameSaveData();
	EARLY_RETURN_IF(!SaveGame)

	SaveGame->SaveProgress(CheckpointTag, this);
	GameMode->SaveInGameProgress(SaveGame);
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

void AP16Character::InitDefaultAttributes() const
{
	// The attributes will be loaded from the save game.
	const AP16GameMode* GameMode = GetWorld()->GetAuthGameMode<AP16GameMode>();
	EARLY_RETURN_IF(!GameMode)
	GameMode->LoadWorldState(GetWorld());

	UP16SaveGame* SaveGame = GameMode->GetInGameSaveData();
	EARLY_RETURN_WITH_WARN_IF(!SaveGame)
	SaveGame->LoadPlayerObject(GetPlayerState());

	if (UP16AbilitySystemComponent* AbilitySystem = Cast<UP16AbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AbilitySystem->AddCharacterAbilitiesFromSaveData(SaveGame);
	}

	if (!SaveGame->GameObject.bFirstLoad)
	{
		UP16AbilitySystemLibrary::InitDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveGame);
		ApplyEffectToSelf(DefaultSecondaryAttributes);
		ApplyEffectToSelf(DefaultVitalAttributes);
		return;
	}

	Super::InitDefaultAttributes();
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
