// Copyright © 2025, Ivan Kinder

#include "Character/P16Character.h"

#include "AbilitySystemComponent.h"
#include "Project16.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/P16PlayerState.h"
#include "UI/HUD/P16HUD.h"

AP16Character::AP16Character()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate              = FRotator{0.f, 400.f, 0.f};
	GetCharacterMovement()->bConstrainToPlane         = true;
	GetCharacterMovement()->bSnapToPlaneAtStart       = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator{-45.f, 0.f, 0.f});
	SpringArm->TargetArmLength         = 750.0f;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bEnableCameraLag        = true;
	SpringArm->bInheritPitch           = false;
	SpringArm->bInheritYaw             = false;
	SpringArm->bInheritRoll            = false;

	Camera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	Camera->SetupAttachment(SpringArm.Get());
	Camera->bUsePawnControlRotation = false;
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

UAttributeSet* AP16Character::GetAttributeSet() const
{
	return AttributeSet.Get();
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
}
