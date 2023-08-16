// Copyright (C) 2023, IKinder

#include "P9/Public/Character/P9PunchCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P9/Public/AnimNotify/P9NotifyWhoosh.h"
#include "P9/Public/AnimNotify/P9NotifyWindowPunch.h"
#include "P9/Public/Interface/P9Interaction.h"
#include "P9/Public/UI/P9HUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogP9PunchCharacter, All, All)

AP9PunchCharacter::AP9PunchCharacter()
{
	/* Enable tick */
	PrimaryActorTick.bCanEverTick = true;
	/* Set size for collision capsule */
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	/* Don`t rotate when the controller rotates. Let that just affect the camera */
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	/* Create and setup components, find default hard references */
	SetupComponents();
	SetupDefaultReferences();
}

void AP9PunchCharacter::BeginPlay()
{
	Super::BeginPlay();
	CheckHardReferences();
	AddMappingContext();
	CallbackAnimNotifies();
	CallbackDelegates();
	SetPossessedColor();
}

void AP9PunchCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, FString::Printf(TEXT("%f"), GetCharacterMovement()->MaxWalkSpeed));
}

void AP9PunchCharacter::SetupComponents()
{
	/* Configure character movement */
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = MaxCrouchSpeed;
	/* Create camera boom */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoomComponent");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;
	/* Create camera */
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCameraComponent");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	/* Left fist Punch collisions */
	LeftFistCollision = CreateDefaultSubobject<USphereComponent>("LeftFistCollisionComponent");
	LeftFistCollision->SetupAttachment(GetMesh(), "SupportPoint");
	LeftFistCollision->SetSphereRadius(10.f);
	LeftFistCollision->AddLocalOffset(FVector(6.f, 2.f, 0.f));
	LeftFistCollision->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	LeftFistCollision->SetNotifyRigidBodyCollision(false);
	LeftFistCollision->SetGenerateOverlapEvents(false);
	/* Right fist Punch collisions */
	RightFistCollision = CreateDefaultSubobject<USphereComponent>("RightFistCollisionComponent");
	RightFistCollision->SetupAttachment(GetMesh(), "GripPoint");
	RightFistCollision->SetSphereRadius(10.f);;
	RightFistCollision->AddLocalOffset(FVector(-2.f, 1.f, -1.f));
	RightFistCollision->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	RightFistCollision->SetNotifyRigidBodyCollision(false);
	RightFistCollision->SetGenerateOverlapEvents(false);
	/* Left leg Kick collisions */
	LeftLegCollision = CreateDefaultSubobject<USphereComponent>("LeftLegCollisionComponent");
	LeftLegCollision->SetupAttachment(GetMesh(), "LeftBall");
	LeftLegCollision->SetSphereRadius(10.f);
	LeftLegCollision->AddLocalOffset(FVector::ZeroVector);
	LeftLegCollision->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	LeftLegCollision->SetNotifyRigidBodyCollision(false);
	LeftLegCollision->SetGenerateOverlapEvents(false);
	/* Right leg Kick collisions */
	RightLegCollision = CreateDefaultSubobject<USphereComponent>("RightLegCollisionComponent");
	RightLegCollision->SetupAttachment(GetMesh(), "RightBall");
	RightLegCollision->SetSphereRadius(10.f);;
	RightLegCollision->AddLocalOffset(FVector::ZeroVector);
	RightLegCollision->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	RightLegCollision->SetNotifyRigidBodyCollision(false);
	RightLegCollision->SetGenerateOverlapEvents(false);
	/* Audio Component */
	PunchAudioComp = CreateDefaultSubobject<UAudioComponent>("PunchAudioComponent");
	PunchAudioComp->SetupAttachment(RootComponent);
	WhooshAudioComp = CreateDefaultSubobject<UAudioComponent>("SwooshAudioComponent");
	WhooshAudioComp->SetupAttachment(RootComponent);	
}

void AP9PunchCharacter::SetupDefaultReferences()
{
	/* Load default hard references. */
	static ConstructorHelpers::FObjectFinder<UDataTable> PlayerAttackMontageDataObject(TEXT("/Script/Engine.DataTable'/Game/Project/P9/DataTable/DT_PlayerAttackMontages.DT_PlayerAttackMontages'"));
	if (PlayerAttackMontageDataObject.Succeeded())
	{
		PlayerAttackDataTable = PlayerAttackMontageDataObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> PunchSoundObject(TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Project/P9/Audio/SFX_Punch.SFX_Punch'"));
	if (PunchSoundObject.Succeeded())
	{
		PunchSound = PunchSoundObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> SwooshSoundObject(TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Project/P9/Audio/SFX_Swoosh.SFX_Swoosh'"));
	if (SwooshSoundObject.Succeeded())
	{
		WhooshSound = SwooshSoundObject.Object;
	}
}

void AP9PunchCharacter::CheckHardReferences()
{
	/* Check Inputs*/
	check(DefaultContext)
	check(MoveAction)
	check(LookAction)
	check(JumpAction)
	check(InteractAction)
	check(CrouchAction)
	check(RunAction)
	check(ArmAction)
	
	/* Check Montages */
	check(PlayerAttackDataTable)

	/* Check Sounds */
	check(PunchAudioComp)
	check(WhooshAudioComp)
	check(PunchSound)
	check(WhooshSound)
}

void AP9PunchCharacter::AddMappingContext()
{
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem) return;
	Subsystem->AddMappingContext(DefaultContext, 0);
}

void AP9PunchCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	auto* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) return;
	/* Move & look. */
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);
	/* Jump. */
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &Super::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &Super::StopJumping);
	/* Crouch. */
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::CrouchInput, true);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ThisClass::CrouchInput, false);
	/* Run. */
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ThisClass::RunInput, true);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ThisClass::RunInput, false);
	/* Attack. */
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::AttackInput);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::InteractInput);
	EnhancedInputComponent->BindAction(ArmAction, ETriggerEvent::Started, this, &ThisClass::ArmInput);
}

void AP9PunchCharacter::MoveInput(const FInputActionValue& Value)
{
	if (!Controller) return;
	
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator YawRotation = FRotator(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// FVector Forward;
	// FVector Right;
	
	if (CharState == EP9CharState::ARMED && CharMoveState != EP9CharMoving::CROUCH)
	{
		if (!GetCharacterMovement()->bUseControllerDesiredRotation)
		{
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
		// Forward = GetActorForwardVector();
		// Right = GetActorRightVector();
	}
	else
	{
		if (GetCharacterMovement()->bUseControllerDesiredRotation)
		{
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}
		// Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	}
	
	AddMovementInput(Forward, MovementVector.Y);
	AddMovementInput(Right, MovementVector.X);
}

void AP9PunchCharacter::LookInput(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AP9PunchCharacter::CrouchInput(const bool bEnable)
{
	bEnable ? Crouch() : UnCrouch();
	CharMoveState = bEnable ? EP9CharMoving::CROUCH : EP9CharMoving::WALK;
	ChangeMovingSpeed();
}

void AP9PunchCharacter::RunInput(const bool bEnable)
{
	if (CharState >= EP9CharState::ARMED || CharMoveState <= EP9CharMoving::ARMED) return;
	CharMoveState = bEnable ? EP9CharMoving::RUN : EP9CharMoving::WALK;
	ChangeMovingSpeed();
}

void AP9PunchCharacter::ArmInput()
{
	const bool bArmed = CharMoveState == EP9CharMoving::ARMED; 
	ArmPlayer(!bArmed);
}

void AP9PunchCharacter::AttackInput()
{
	/* Prevent spam multiple attacks. */
	if (CharState >= EP9CharState::ATTACKING) return;
	
	/* Find random table row key name and use it to get AnimMontage data from DataTable row. */
	const int RandomKeyNum = FMath::RandRange(0, PlayerAttackDataTable->GetRowNames().Num() - 1);
	const FName RandomKey = PlayerAttackDataTable->GetRowNames()[RandomKeyNum];
	const auto* AttackMontageRow = PlayerAttackDataTable->FindRow<FP9PlayerAttackMontage>(RandomKey, "", true);
	if (!AttackMontageRow) return;
	UAnimMontage* AnimMontage = AttackMontageRow->Montage;
	if (!AnimMontage) return;

	/* To play animation we need to get any section name without last symbol. Then generate random number and use it as SectionName. */
	const FString MontageSectionName = AnimMontage->GetSectionName(0).ToString().LeftChop(1);
	const int MontageSectionIndex = FMath::RandRange(1, AnimMontage->GetNumSections());
	PlayAnimMontage(AnimMontage, 1.f, FName(MontageSectionName + FString::FromInt(MontageSectionIndex)));
	
	CharState = EP9CharState::ATTACKING;
	/* OnPunchHandle should be fired next, after the anim notify is triggered. */
		
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("RowName: %s, Montage: %s"), *RandomKey.ToString(), *(MontageSectionName + FString::FromInt(MontageSectionIndex))));
}

void AP9PunchCharacter::InteractInput()
{
	FVector Start;
	FVector End;
	const FVector CameraStart = FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * CameraBoom->TargetArmLength;
	const FVector PlayerEyeStart = GetActorLocation() + FVector(0.f, 0.f, BaseEyeHeight);
	
	Start = LineTraceType == EP9LineTraceType::CAMERA ? CameraStart : PlayerEyeStart;
	End = Start + FollowCamera->GetForwardVector() * InteractDistance;
	
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Camera, Params))
	{
		if (auto InteractInterface = Cast<IP9Interaction>(HitResult.GetActor()))
		{
			if (!SavedController) SavedController = GetController();
			InteractInterface->Interact(this);
		}
	}
	TraceDebugLine(HitResult, Start, End);
}

void AP9PunchCharacter::Interact(ACharacter* Causer)
{
	if (!Causer) return;
	AP9PunchCharacter* PunchCauser = Cast<AP9PunchCharacter>(Causer);
	if (!PunchCauser) return;
	AController* CauserSavedController = PunchCauser->GetSavedController();
	if (!CauserSavedController) return;

	/* UnPossess previous pawn with all consequences. */
	CauserSavedController->UnPossess();
	PunchCauser->GetCharacterMovement()->SetMovementMode(MOVE_None);
	PunchCauser->SetPossessedColor();

	/* Possess current pawn with all consequences. */
	CauserSavedController->Possess(this);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	SetPossessedColor();
}

void AP9PunchCharacter::SetPossessedColor()
{
	bCurrentlyPosses = Cast<APlayerController>(Controller) != nullptr;

	UMaterialInstanceDynamic* MaterialInstanceDynamic1 = GetMesh()->CreateAndSetMaterialInstanceDynamic(1);
	UMaterialInstanceDynamic* MaterialInstanceDynamic2 = GetMesh()->CreateAndSetMaterialInstanceDynamic(2);
	UMaterialInstanceDynamic* MaterialInstanceDynamic3 = GetMesh()->CreateAndSetMaterialInstanceDynamic(3);
	if (!MaterialInstanceDynamic1 || !MaterialInstanceDynamic2 || !MaterialInstanceDynamic3) return;

	const FColor NewColor = bCurrentlyPosses ? PlayerColor.Possessed : PlayerColor.UnPossessed;
	for (const FName ColorName : PossessedColorParameterNames)
	{
		MaterialInstanceDynamic1->SetVectorParameterValue(ColorName, NewColor);
		MaterialInstanceDynamic2->SetVectorParameterValue(ColorName, NewColor);
		MaterialInstanceDynamic3->SetVectorParameterValue(ColorName, NewColor);
	}
}

void AP9PunchCharacter::OnPunchHandle(USkeletalMeshComponent* MeshComp, const bool bStart)
{
	if (MeshComp != GetMesh()) return;
	/* Setting Fists collisions depending on the input condition. */
	const FName ProfileName = bStart ? MeleeCollisionProfile.Enabled : MeleeCollisionProfile.Disabled;
	LeftFistCollision->SetCollisionProfileName(ProfileName);
	LeftFistCollision->SetNotifyRigidBodyCollision(bStart);
	LeftFistCollision->SetGenerateOverlapEvents(bStart);
	
	RightFistCollision->SetCollisionProfileName(ProfileName);
	RightFistCollision->SetNotifyRigidBodyCollision(bStart);
	RightFistCollision->SetGenerateOverlapEvents(bStart);

	LeftLegCollision->SetCollisionProfileName(ProfileName);
	LeftLegCollision->SetNotifyRigidBodyCollision(bStart);
	LeftLegCollision->SetGenerateOverlapEvents(bStart);
	
	RightLegCollision->SetCollisionProfileName(ProfileName);
	RightLegCollision->SetNotifyRigidBodyCollision(bStart);
	RightLegCollision->SetGenerateOverlapEvents(bStart);
	
	/* When the attack window is closed, set character state to ARMED, and after ArmedToIdleDelay - change state to IDLE. */
	if (!bStart)
	{
		ArmPlayer(true);
		
		FTimerDelegate ArmDelegate;
		ArmDelegate.BindLambda([&]()
		{
			ArmPlayer(false);
		});
		GetWorld()->GetTimerManager().SetTimer(ArmTimer, ArmDelegate, ArmedToIdleDelay, false);
	}
}

void AP9PunchCharacter::OnWhooshHandle(USkeletalMeshComponent* MeshComp)
{
	if (!WhooshAudioComp->GetSound()) WhooshAudioComp->SetSound(WhooshSound);
	WhooshAudioComp->Play();
}

void AP9PunchCharacter::OnAttackHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == this || CharState >= EP9CharState::PUNCHED) return;
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("%hs: %s"), __FUNCTION__, *Hit.GetActor()->GetActorNameOrLabel()));
	/* Play punch sound */
	if (!PunchAudioComp->GetSound()) PunchAudioComp->SetSound(PunchSound);
	PunchAudioComp->Play();
	/* Stop anim montage */
	StopAnimMontage();
	CharState = EP9CharState::PUNCHED;

	/* Get HUD reference and increment combo count */
	if (const auto PC = Cast<APlayerController>(Controller))
	{
		HUD = Cast<AP9HUD>(PC->GetHUD());
		if (!HUD) return;

		HUD->UpdateComboCount(++CurrentComboCount);

		/* Update combo timer. */
		if (GetWorld()->GetTimerManager().IsTimerActive(ComboTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(ComboTimer);
		}
		GetWorld()->GetTimerManager().SetTimer(ComboTimer, this, &ThisClass::ResetComboHandle, 5.f);
	}
}

void AP9PunchCharacter::OnAttackBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("%hs: %s"), __FUNCTION__, *OtherActor->GetActorNameOrLabel()));
}

void AP9PunchCharacter::OnAttackEndOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == this) return;
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("%hs: %s"), __FUNCTION__, *OtherActor->GetActorNameOrLabel()));
}

void AP9PunchCharacter::CallbackAnimNotifies()
{
	TArray<FP9PlayerAttackMontage*> Rows;
	PlayerAttackDataTable->GetAllRows("", OUT Rows);
	for (const FP9PlayerAttackMontage* Row : Rows)
	{
		if (!Row->Montage) continue;
		for (const FAnimNotifyEvent& NotifyEvent : Row->Montage->Notifies)
		{
			if (UP9NotifyWindowPunch* NotifyWindowPunch = Cast<UP9NotifyWindowPunch>(NotifyEvent.NotifyStateClass))
			{
				NotifyWindowPunch->OnPunchStart.AddUObject<ThisClass, bool>(this, &ThisClass::OnPunchHandle, true);
				NotifyWindowPunch->OnPunchEnd.AddUObject<ThisClass, bool>(this, &ThisClass::OnPunchHandle, false);
			}
			if (UP9NotifyWhoosh* NotifySwoosh = Cast<UP9NotifyWhoosh>(NotifyEvent.Notify))
			{
				NotifySwoosh->OnWhoosh.AddUObject(this, &ThisClass::OnWhooshHandle);
			}
		}
	}
}

void AP9PunchCharacter::CallbackDelegates()
{
	LeftFistCollision->OnComponentHit.AddDynamic(this, &ThisClass::OnAttackHitHandle);
	LeftFistCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnAttackBeginOverlapHandle);
	LeftFistCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnAttackEndOverlapHandle);
	
	RightFistCollision->OnComponentHit.AddDynamic(this, &ThisClass::OnAttackHitHandle);
	RightFistCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnAttackBeginOverlapHandle);
	RightFistCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnAttackEndOverlapHandle);

	LeftLegCollision->OnComponentHit.AddDynamic(this, &ThisClass::OnAttackHitHandle);
	LeftLegCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnAttackBeginOverlapHandle);
	LeftLegCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnAttackEndOverlapHandle);
	
	RightLegCollision->OnComponentHit.AddDynamic(this, &ThisClass::OnAttackHitHandle);
	RightLegCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnAttackBeginOverlapHandle);
	RightLegCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnAttackEndOverlapHandle);
}

void AP9PunchCharacter::ResetComboHandle()
{
	CurrentComboCount = 0;
	HUD->ResetCombo();
}

void AP9PunchCharacter::ArmPlayer(const bool bEnable)
{
	if (bEnable)
	{
		CharState = EP9CharState::ARMED;
		CharMoveState = EP9CharMoving::ARMED;
	}
	else
	{
		CharState = EP9CharState::IDLE;
		CharMoveState = EP9CharMoving::WALK;
	}
	ChangeMovingSpeed();
}

void AP9PunchCharacter::ChangeMovingSpeed() const
{
	switch (CharMoveState)
	{
	case EP9CharMoving::CROUCH: GetCharacterMovement()->MaxWalkSpeed = MaxCrouchSpeed; break;
	case EP9CharMoving::ARMED:  GetCharacterMovement()->MaxWalkSpeed = MaxArmedSpeed;  break;
	case EP9CharMoving::WALK:   GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;   break;
	case EP9CharMoving::RUN:    GetCharacterMovement()->MaxWalkSpeed = MaxRunSpeed;    break;
	default:                    GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	}
}

void AP9PunchCharacter::Log(const EP9LogLevel Level, const FString& Message)
{
	Log(Level, Message, EP9LogOutput::ALL);
}

void AP9PunchCharacter::Log(const EP9LogLevel Level, const FString& Message, const EP9LogOutput LogOutput)
{
	/* Only print when screen is selected and GEngine object is available */
	if (GEngine &&(LogOutput == EP9LogOutput::ALL || LogOutput == EP9LogOutput::SCREEN))
	{
		/* Default color */
		FColor LogColor;
		/* Flip the color based on the type */
		if (Level == EP9LogLevel::TRACE)        LogColor = FColor::Green;
		else if (Level == EP9LogLevel::DEBUG)   LogColor = FColor::Cyan;
		else if (Level == EP9LogLevel::INFO)    LogColor = FColor::White;
		else if (Level == EP9LogLevel::WARNING) LogColor = FColor::Yellow;
		else if (Level == EP9LogLevel::ERROR)   LogColor = FColor::Red;
		/* Print the message and leave it on screen */
		GEngine->AddOnScreenDebugMessage(-1, 5.f, LogColor, Message);
	}
	if (LogOutput == EP9LogOutput::ALL || LogOutput == EP9LogOutput::LOG)
	{
		// flip the message type based on error level
		switch (Level)
		{
		case EP9LogLevel::TRACE:   UE_LOG(LogP9PunchCharacter, VeryVerbose, TEXT("%s"), *Message); break;
		case EP9LogLevel::DEBUG:   UE_LOG(LogP9PunchCharacter, Verbose, TEXT("%s"), *Message);     break;
		case EP9LogLevel::INFO:    UE_LOG(LogP9PunchCharacter, Log, TEXT("%s"), *Message);         break;
		case EP9LogLevel::WARNING: UE_LOG(LogP9PunchCharacter, Warning, TEXT("%s"), *Message);     break;
		case EP9LogLevel::ERROR:   UE_LOG(LogP9PunchCharacter, Error, TEXT("%s"), *Message);       break;
		default:                   UE_LOG(LogP9PunchCharacter, Log, TEXT("%s"), *Message);         break;
		}
	}
}

void AP9PunchCharacter::TraceDebugLine(const FHitResult& Hit, const FVector& Start, const FVector& End) const
{
	if (!Hit.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 10.f);
		return;
	}
	DrawDebugLine(GetWorld(), Start, Hit.ImpactPoint, FColor::Green, false, 10.f);
	DrawDebugLine(GetWorld(), Hit.ImpactPoint, End, FColor::Red, false, 10.f);
	DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 10.f, FColor::Red, false, 10.f);
}