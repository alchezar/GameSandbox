// Copyright © 2025, Ivan Kinder

#include "Player/P15Character.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Project15.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/Abilities/P15BaseAbility.h"
#include "Gameplay/Attribute/P15AttributeSet.h"
#include "Player/P15HUD.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/BrainComponent.h"

AP15Character::AP15Character()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("SpringArmCameraBoomComponent");
	CameraBoom->SetupAttachment(RootComponent.Get());

	PlayerEye = CreateDefaultSubobject<UCameraComponent>("PlayerEyeCameraComponent");
	PlayerEye->SetupAttachment(CameraBoom.Get());

	AttributeSet = CreateDefaultSubobject<UP15AttributeSet>("AttributeSet");

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed             = P15::WalkSpeed;
		Movement->NavAgentProps.bCanCrouch = true;
		Movement->SetCrouchedHalfHeight(66.f);

		MaxCrouchOffset = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - GetCharacterMovement()->GetCrouchedHalfHeight();
	}
}

void AP15Character::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Use one Anim Blueprint for all skeletal meshes.
	EARLY_RETURN_IF(!GetMesh())
	for (const TObjectPtr<USceneComponent>& MeshChild : GetMesh()->GetAttachChildren())
	{
		USkinnedMeshComponent* SkeletalChild = Cast<USkinnedMeshComponent>(MeshChild);
		CONTINUE_IF(!SkeletalChild)

		SkeletalChild->SetLeaderPoseComponent(GetMesh());
	}
}

void AP15Character::BeginPlay()
{
	Super::BeginPlay();

	AddDefaultMappingContext();
	AutoDetermineTeamID();

	AcquireAllAbilities();
	AddAbilitiesToUI();

	AttributeSet->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChangedCallback);
	AttributeSet->OnManaChanged.AddUObject(this, &ThisClass::OnManaChangedCallback);
	AttributeSet->OnStrengthChanged.AddUObject(this, &ThisClass::OnStrengthChangedCallback);
	AddGameplayTag(FullHealthTag);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCapsuleBeginOverlapCallback);
}

void AP15Character::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChangeWalkSpeedSmoothly(DeltaTime);
	UpdateCameraBoomOffsetSmoothly(DeltaTime);
}

void AP15Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// clang-format off
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction.Get(),   ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
		Input->BindAction(MoveAction.Get(),   ETriggerEvent::Started,   this, &ThisClass::ChangeMovementState, true);
		Input->BindAction(MoveAction.Get(),   ETriggerEvent::Completed, this, &ThisClass::ChangeMovementState, false);
		Input->BindAction(LookAction.Get(),   ETriggerEvent::Triggered, this, &ThisClass::LookInput);
		Input->BindAction(JumpAction.Get(),   ETriggerEvent::Started,   this, &ThisClass::JumpInput, true);
		Input->BindAction(JumpAction.Get(),   ETriggerEvent::Completed, this, &ThisClass::JumpInput, false);
		Input->BindAction(RunAction.Get(),    ETriggerEvent::Started,   this, &ThisClass::RunInput, true);
		Input->BindAction(RunAction.Get(),    ETriggerEvent::Completed, this, &ThisClass::RunInput, false);
		Input->BindAction(CrouchAction.Get(), ETriggerEvent::Started,   this, &ThisClass::CrouchInput);
		Input->BindAction(AttackAction.Get(), ETriggerEvent::Started,   this, &ThisClass::PushInput);
		Input->BindAction(AimAction.Get(),    ETriggerEvent::Started,   this, &ThisClass::AttackInput, true);
		Input->BindAction(AimAction.Get(),    ETriggerEvent::Completed, this, &ThisClass::AttackInput, false);
		Input->BindAction(RegenAction.Get(),  ETriggerEvent::Completed, this, &ThisClass::RegenInput);
		Input->BindAction(DashAction.Get(),   ETriggerEvent::Completed, this, &ThisClass::DashInput);
		Input->BindAction(LaserAction.Get(),  ETriggerEvent::Completed, this, &ThisClass::LaserInput);
		Input->BindAction(BlastAction.Get(),  ETriggerEvent::Completed, this, &ThisClass::BlastInput);
	}
	// clang-format on
}

void AP15Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PlayerController = Cast<APlayerController>(NewController);
}

UAbilitySystemComponent* AP15Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComp.Get();
}

void AP15Character::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	EARLY_RETURN_IF(!AbilitySystemComp)
	AbilitySystemComp->GetOwnedGameplayTags(TagContainer);
}

void AP15Character::SetCollisionResponseToPawn(const ECollisionResponse NewResponse)
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, NewResponse);
}

void AP15Character::SetTargetingState(const bool bNewTargeting)
{
	bTargeting = bNewTargeting;
}

bool AP15Character::GetIsHostile(const AP15Character* Other) const
{
	EARLY_RETURN_VALUE_IF(!Other, false)
	return TeamID != Other->GetTeamID();
}

void AP15Character::AddGameplayTag(const FGameplayTag TagToAdd) const
{
	AbilitySystemComp->AddLooseGameplayTag(TagToAdd);
}

void AP15Character::RemoveGameplayTag(const FGameplayTag TagToRemove) const
{
	AbilitySystemComp->RemoveLooseGameplayTag(TagToRemove);
}

void AP15Character::ToggleGameplayTag(const FGameplayTag TagToToggle, const bool bAdd) const
{
	AbilitySystemComp->SetTagMapCount(TagToToggle, bAdd ? 1 : 0);
}

void AP15Character::PushCharacter(const FVector& Direction, const float Strength, const float Duration)
{
	// Add impulse to character.
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	EARLY_RETURN_IF(!MovementComp)
	MovementComp->AddImpulse(Direction * Strength, true);

	// Disable ground friction for a push duration.
	EARLY_RETURN_IF(Duration <= 0.f)
	Stun(Duration);
}

void AP15Character::Stun(const float Duration)
{
	if (PlayerController)
	{
		PlayerController->DisableInput(PlayerController);
	}
	else if (const AAIController* AIController = Cast<AAIController>(Controller))
	{
		AIController->GetBrainComponent()->StopLogic("Stun");
	}
	GetCharacterMovement()->GroundFriction = 0.f;

	const FTimerDelegate StunDelegate = FTimerDelegate::CreateWeakLambda(this, [this]() -> void
	{
		if (PlayerController)
		{
			PlayerController->EnableInput(PlayerController);
		}
		else if (const AAIController* AIController = Cast<AAIController>(Controller))
		{
			AIController->GetBrainComponent()->RestartLogic();
		}
		GetCharacterMovement()->GroundFriction = P15::GroundFriction;
	});
	GetWorld()->GetTimerManager().SetTimer(StunTimer, StunDelegate, Duration, false);
}

void AP15Character::MoveInput(const FInputActionValue& InputValue)
{
	EARLY_RETURN_IF(!PlayerController || !PlayerController->InputEnabled())
	const FVector ForwardDirection = FRotator{0.f, Controller->GetControlRotation().Yaw, 0.f}.RotateVector(FVector::ForwardVector);
	const FVector RightDirection   = FRotator{0.f, Controller->GetControlRotation().Yaw, 0.f}.RotateVector(FVector::RightVector);

	const FVector2D InputVector = InputValue.Get<FVector2D>();
	AddMovementInput(ForwardDirection, InputVector.Y);
	AddMovementInput(RightDirection, InputVector.X);
}

void AP15Character::JumpInput(const bool bInAir)
{
	bFalling = bInAir;
	bFalling ? Jump() : StopJumping();
}

void AP15Character::ChangeMovementState(const bool bStart)
{
	bMovingInput = bStart;

	GetCharacterMovement()->bUseControllerDesiredRotation = bMovingInput && !bFalling;
}

void AP15Character::LookInput(const FInputActionValue& InputValue)
{
	EARLY_RETURN_IF(!Controller)
	const FVector2D InputVector = InputValue.Get<FVector2D>();

	AddControllerYawInput(InputVector.X);
	AddControllerPitchInput(InputVector.Y);
}

void AP15Character::RunInput(const bool bRun)
{
	SpeedChangeData.bActive   = true;
	SpeedChangeData.bPositive = bRun;
	SpeedChangeData.Alpha     = 0.f;
}

void AP15Character::CrouchInput()
{
	CameraBoom->bEnableCameraLag = false;
	CameraBoom->SocketOffset.Z   = bIsCrouched ? -MaxCrouchOffset : MaxCrouchOffset;
	bIsCrouched ? UnCrouch() : Crouch();

	CameraOffsetChangeData.bActive   = true;
	CameraOffsetChangeData.bPositive = !bIsCrouched;
	CameraOffsetChangeData.Alpha     = 0.f;
}

void AP15Character::PushInput()
{
	// Confirming the target for the target ability if it's active.
	if (bTargeting)
	{
		AbilitySystemComp->TargetConfirm();
		return;
	}

	// Default behavior for the push input.
	AbilitySystemComp->TryActivateAbilityByClass(PushAbility);
}

void AP15Character::AttackInput(const bool bStart)
{
	// Canceling the target for the target ability if it's active.
	if (bTargeting)
	{
		AbilitySystemComp->TargetCancel();
		return;
	}

	if (bStart)
	{
		// Activate ability to start playing attack animation.
		AbilitySystemComp->TryActivateAbilityByClass(MeleeAbility);
	}
	else
	{
		// Try to deal damage at input completion.
		FGameplayEventData Payload;
		Payload.Instigator = this;
		FGameplayTag Tag   = FGameplayTag::RequestGameplayTag("p15.skill.melee.deal_damage");
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, MoveTemp(Tag), MoveTemp(Payload));
	}
}

void AP15Character::RegenInput()
{
	ActivateAbility(RegenAbility);
}

void AP15Character::DashInput()
{
	ActivateAbility(DashAbility);
}

void AP15Character::LaserInput()
{
	ActivateAbility(LaserAbility);

	// Prevent mana drain.
	FTimerHandle   LaserTimer;
	FTimerDelegate LaserDelegate = FTimerDelegate::CreateWeakLambda(this, [this]() -> void
	{
		AbilitySystemComp->CancelAbility(LaserAbility.GetDefaultObject());
	});
	GetWorld()->GetTimerManager().SetTimer(LaserTimer, MoveTemp(LaserDelegate), LaserAbility.GetDefaultObject()->GetAbilityInfo().CooldownDuration, false);
}

void AP15Character::BlastInput()
{
	ActivateAbility(BlastAbility, false);
}

void AP15Character::OnHealthChangedCallback(const float NewHealthPercentage)
{
	ToggleGameplayTag(FullHealthTag, FMath::IsNearlyEqual(NewHealthPercentage, 1));

	EARLY_RETURN_IF(bDead || NewHealthPercentage > 0.f)

	bDead = true;
	AbilitySystemComp->TryActivateAbilityByClass(DeadAbility);
}

void AP15Character::OnManaChangedCallback(const float NewManaPercentage)
{}

void AP15Character::OnStrengthChangedCallback(const float NewStrengthPercentage)
{}

void AP15Character::OnCapsuleBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EARLY_RETURN_IF(!OtherActor->IsA(AP15Character::StaticClass()))

	FGameplayEventData Payload;
	Payload.Instigator = this;
	Payload.Target     = OtherActor;
	FGameplayTag Tag   = FGameplayTag::RequestGameplayTag("p15.skill.dash.deal_damage");
	AbilitySystemComp->HandleGameplayEvent(MoveTemp(Tag), &Payload);
}

void AP15Character::AddDefaultMappingContext() const
{
	EARLY_RETURN_IF(!PlayerController)
	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	EARLY_RETURN_IF(!LocalPlayer)
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	EARLY_RETURN_IF(!Subsystem)

	Subsystem->AddMappingContext(InputContext, 0);
}

void AP15Character::AutoDetermineTeamID()
{
	EARLY_RETURN_IF(!GetController() || !GetController()->IsPlayerController())
	TeamID = 0;
}

void AP15Character::AcquireAllAbilities()
{
	EARLY_RETURN_IF(!AbilitySystemComp)

	// Find all ability classes in this class.
	for (TFieldIterator<FProperty> It{this->GetClass()}; It; ++It)
	{
		FProperty*            Property      = *It;
		const FClassProperty* ClassProperty = CastField<FClassProperty>(Property);
		CONTINUE_IF(!ClassProperty || !ClassProperty->MetaClass || !ClassProperty->MetaClass->IsChildOf(UP15BaseAbility::StaticClass()))

		const TSubclassOf<UP15BaseAbility>* AbilityClassPtr = ClassProperty->ContainerPtrToValuePtr<TSubclassOf<UP15BaseAbility>>(this);
		CONTINUE_IF(!AbilityClassPtr)
		TSubclassOf<UP15BaseAbility> AbilityClass = *AbilityClassPtr;
		CONTINUE_IF(!AbilityClass)

		AllAbilities.Add(AbilityClass);
	}

	// Acquire all abilities.
	if (HasAuthority())
	{
		for (const TSubclassOf<UP15BaseAbility>& AbilityToAcquire : AllAbilities)
		{
			AbilitySystemComp->GiveAbility(FGameplayAbilitySpec{AbilityToAcquire});
		}
	}
	AbilitySystemComp->InitAbilityActorInfo(this, this);
}

void AP15Character::AddAbilitiesToUI()
{
	EARLY_RETURN_IF(!PlayerController)
	const AP15HUD* PlayerHUD = PlayerController->GetHUD<AP15HUD>();
	EARLY_RETURN_IF(!PlayerHUD)

	// Filter by ability classes that have textures.
	auto Predicate = [](const TSubclassOf<UP15BaseAbility>& Ability) -> bool
	{
		return Ability
			&& Ability->GetDefaultObject<UP15BaseAbility>()
			&& Ability->GetDefaultObject<UP15BaseAbility>()->GetHasTexture();
	};
	TArray<TSubclassOf<UP15BaseAbility>> FilteredAbilities = AllAbilities.FilterByPredicate(Predicate);

	// Add filtered abilities to the UI.
	// The order of the ability classes here will correspond to the order of the UI slots.
	for (int32 Index = 0; Index < FilteredAbilities.Num(); ++Index)
	{
		UP15BaseAbility* AbilityInstance = FilteredAbilities[Index].Get()->GetDefaultObject<UP15BaseAbility>();
		CONTINUE_IF(!AbilityInstance)

		FP15AbilityInfo AbilityInfo = AbilityInstance->GetAbilityInfo();
		PlayerHUD->AddAbilityToUI(Index, MoveTemp(AbilityInfo), &OnAbilityStarted);
	}
}

void AP15Character::ActivateAbility(const TSubclassOf<UP15BaseAbility>& AbilityClass, const bool bImmediately) const
{
	if (AbilitySystemComp->TryActivateAbilityByClass(AbilityClass) && bImmediately)
	{
		OnAbilityStarted.Broadcast(AbilityClass);
	}
}

void AP15Character::ChangeWalkSpeedSmoothly(const float DeltaTime)
{
	EARLY_RETURN_IF(!SpeedChangeData.bActive)

	float&      CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	const float StartSpeed   = SpeedChangeData.bPositive ? P15::WalkSpeed : P15::RunSpeed;
	const float TargetSpeed  = SpeedChangeData.bPositive ? P15::RunSpeed : P15::WalkSpeed;

	float& Alpha = SpeedChangeData.Alpha;
	if (Alpha >= 1.f)
	{
		SpeedChangeData.bActive = false;
		CurrentSpeed            = TargetSpeed;
		return;
	}

	CurrentSpeed = FMath::InterpSinOut(StartSpeed, TargetSpeed, Alpha);
	Alpha += DeltaTime / CameraOffsetChangeData.Time;
}

void AP15Character::UpdateCameraBoomOffsetSmoothly(const float DeltaTime)
{
	EARLY_RETURN_IF(!CameraOffsetChangeData.bActive)

	double&          CurrentOffset = CameraBoom->SocketOffset.Z;
	const double     StartOffset   = CameraOffsetChangeData.bPositive ? MaxCrouchOffset : -MaxCrouchOffset;
	constexpr double TargetOffset  = 0.f;

	float& Alpha = CameraOffsetChangeData.Alpha;
	if (Alpha >= 1.f)
	{
		CameraBoom->bEnableCameraLag   = true;
		CameraOffsetChangeData.bActive = false;
		CurrentOffset                  = TargetOffset;
		return;
	}

	CurrentOffset = FMath::InterpSinOut(StartOffset, TargetOffset, Alpha);
	Alpha += DeltaTime / CameraOffsetChangeData.Time;
}
