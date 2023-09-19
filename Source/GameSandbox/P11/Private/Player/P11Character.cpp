// Copyright (C) 2023, IKinder

#include "P11/Public/Player/P11Character.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "P11/Public/Game/P11GameModeBase.h"
#include "P11/Public/Game/P11SavePlayerInfo.h"
#include "P11/Public/Player/P11PlayerState.h"
#include "P11/Public/UI/P11HUD.h"

AP11Character::AP11Character()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	CameraBoom->TargetArmLength = 150.f;
	CameraBoom->SocketOffset = CameraSide == EP11CameraSide::Right ? FVector(0.f, CameraOffset, 0.f) : FVector(0.f, -1 * CameraOffset, 0.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	FollowCamera->SetupAttachment(CameraBoom);

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	
	FindDefaultReferences();
}

void AP11Character::BeginPlay()
{
	Super::BeginPlay();
	GetSubsystem();
	Health = MaxHealth;
	Ammo = MaxAmmo;
}

void AP11Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP11Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	GetWorld()->GetTimerManager().SetTimer(DrawUIHandle, this, &ThisClass::ShowInterface, 1.f, false);
}

void AP11Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, CharState)
	DOREPLIFETIME(ThisClass, MaxHealth)
	DOREPLIFETIME(ThisClass, Health)
	DOREPLIFETIME(ThisClass, MaxAmmo)
	DOREPLIFETIME(ThisClass, Ammo)
	DOREPLIFETIME(ThisClass, PlayerSide)
}

void AP11Character::GetSubsystem()
{
	const auto* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)
	{
		return;
	}
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem)
	{
		return;
	}
	Subsystem->AddMappingContext(DefaultContext, 0);
}

void AP11Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!InputComp)
	{
		return;
	}
	InputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
	InputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);
	InputComp->BindAction(JumpAction, ETriggerEvent::Started, this, &Super::Jump);
	InputComp->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::CrouchInput);
	InputComp->BindAction(FireAction, ETriggerEvent::Started, this, &ThisClass::FireInput);
	InputComp->BindAction(ShiftCameraAction, ETriggerEvent::Started, this, &ThisClass::ShiftCameraInput);
}

void AP11Character::MoveInput(const FInputActionValue& ActionValue)
{
	if (!Controller)
	{
		return;
	}
	const FVector2D MoveVector = ActionValue.Get<FVector2D>();
	const FRotator YawRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, MoveVector.Y);
	AddMovementInput(Right, MoveVector.X);
}

void AP11Character::LookInput(const FInputActionValue& ActionValue)
{
	if (!Controller)
	{
		return;
	}
	const FVector2D LookVector = ActionValue.Get<FVector2D>();
	
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void AP11Character::CrouchInput()
{
	bCrouch = !bCrouch;
	bCrouch ? Crouch() : UnCrouch();
}

void AP11Character::ShowInterface_Implementation()
{
	AP11HUD* HUD = GetPlayerHUD();
	if (!HUD)
	{
		return;
	}
	HUD->ShowUI(true);
}

void AP11Character::HideInterface_Implementation()
{
	AP11HUD* HUD = GetPlayerHUD();
	if (!HUD)
	{
		return;
	}
	HUD->ShowUI(false);
}

void AP11Character::FireInput()
{
	ServerFire();
}

void AP11Character::ServerFire_Implementation()
{
	Fire();
}

void AP11Character::Fire()
{
	/* To avoid using auto-clicker. */
	if (CharState >= EP11CharState::Shoot)
	{
		return;
	}
	EP11CharState StateBeforeShoot = CharState;
	CharState = EP11CharState::Shoot;

	/* Play fire sound depends on ammo availability. */
	if (Ammo <= 0)
	{
		Multicast_Audio(NoAmmoSound);
		return;	
	}
	Multicast_Audio(FireSound);

	/* Decrease ammo amount after every shoot. */
	Ammo = FMath::Clamp(Ammo - 1, 0, MaxAmmo);
	OnAmmoChanged.Broadcast(Ammo);
	
	/* Shot line trace logic. */
	Client_Trace();

	/* Return the ability to shoot. */
	constexpr float PauseBetweenShots = 0.2f;
	FTimerDelegate ShootDelegate;
	ShootDelegate.BindUObject(this, &ThisClass::ReturnShootAbility, StateBeforeShoot);
	GetWorld()->GetTimerManager().SetTimer(ShootHandle, ShootDelegate, PauseBetweenShots, false);
}

void AP11Character::Client_Trace_Implementation()
{
	/* First trace from the client. */
	FHitResult HitResult;
	ShotTrace(HitResult);
	if (HitResult.bBlockingHit && HitResult.GetActor()->IsA(ACharacter::StaticClass()))
	{
		/* Only if we hit a character - trace again from the server. */
		Server_Trace();
	}
}

void AP11Character::Server_Trace_Implementation()
{
	/* Check again if we hit a character. If true - apply damage. */
	FHitResult HitResult;
	ShotTrace(HitResult);
	if (HitResult.bBlockingHit && HitResult.GetActor()->IsA(ACharacter::StaticClass()))
	{
		constexpr float ShotDamage = 25.f;
		UGameplayStatics::ApplyDamage(HitResult.GetActor(), ShotDamage, GetController(), this, nullptr);
	}
}

void AP11Character::ShotTrace(FHitResult& HitResult)
{
	/* Line trace from selected points. */
	const FVector Start = FollowCamera->GetComponentLocation();
	const FVector End = Start + (FollowCamera->GetForwardVector() * 300000.f);
	
	FCollisionQueryParams FireParams;
	FireParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Camera, FireParams);
	DrawDebugShoot(HitResult, 5.f);
}

float AP11Character::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (CharState == EP11CharState::Dead)
	{
		return 0.f;	
	}
	
	const float OldHealth = Health;
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
	
	if (Health == 0.f)
	{
		CharState = EP11CharState::Dead;
		HideInterface();
		Multicast_Ragdoll();
		AddScore(EventInstigator);

		FTimerHandle RespawnTimer;
		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &ThisClass::RespawnHandle, 5.f);
	}
	
	return OldHealth - Health;
}

void AP11Character::Multicast_Ragdoll_Implementation()
{
	GetCapsuleComponent()->DestroyComponent();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->MovementState.bCanJump = false;
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");
}

void AP11Character::RespawnHandle()
{
	AP11GameModeBase* GameMode = Cast<AP11GameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		return;
	}
	AController* DeadCharController = GetInstigatorController();
	if (!DeadCharController)
	{
		return;
	}
	GameMode->Respawn(DeadCharController);
}

void AP11Character::DrawDebugShoot(const FHitResult& HitResult, const float Time) const
{
	if (!HitResult.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), HitResult.TraceStart, HitResult.TraceEnd, FColor::Green, false, Time);
		return;
	}
	DrawDebugLine(GetWorld(), HitResult.TraceStart, HitResult.ImpactPoint, FColor::Red, false, Time);
	DrawDebugLine(GetWorld(), HitResult.ImpactPoint, HitResult.TraceEnd, FColor::Green, false, Time);
	DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.f, FColor::Red, false, Time);
}

void AP11Character::ShiftCameraInput()
{
	float Shift;
	EP11CameraSide NewCameraSide;
	if (CameraSide == EP11CameraSide::Right)
	{
		NewCameraSide = EP11CameraSide::Left;
		Shift = -1.f * CameraOffset;
	}
	else
	{
		NewCameraSide = EP11CameraSide::Right;
		Shift = CameraOffset;
	}
	
	FTimerDelegate CameraShiftDelegate;
	CameraShiftDelegate.BindUObject(this, &ThisClass::ShiftCameraSmoothly, NewCameraSide, Shift);
	GetWorld()->GetTimerManager().SetTimer(CameraShiftTimer, CameraShiftDelegate, GetWorld()->GetDeltaSeconds(), true);
}

void AP11Character::Server_ShiftCamera_Implementation(const EP11CameraSide NewSide, const float NewShift)
{
	Multicast_ShiftCamera(NewSide, NewShift);
}

void AP11Character::Multicast_ShiftCamera_Implementation(const EP11CameraSide NewSide, const float NewShift)
{
	CameraSide = NewSide;
	CameraBoom->SocketOffset = FVector(0.f, NewShift, 0.f);
}

void AP11Character::ShiftCameraSmoothly(const EP11CameraSide NewSide, const float TargetShift)
{
	const FVector LastOffset = CameraBoom->SocketOffset;
	CameraBoom->SocketOffset = FMath::VInterpTo(LastOffset, FVector(0.f, TargetShift, 0.f), GetWorld()->GetDeltaSeconds(), 10.f);
	if (LastOffset.Y == TargetShift)
	{
		GetWorld()->GetTimerManager().ClearTimer(CameraShiftTimer);
		Server_ShiftCamera(NewSide, TargetShift);
	}
}

void AP11Character::Server_Audio_Implementation(USoundBase* SoundToPlay)
{
	Multicast_Audio(SoundToPlay);
}

void AP11Character::Multicast_Audio_Implementation(USoundBase* SoundToPlay)
{
	UGameplayStatics::SpawnSoundAttached(SoundToPlay, GetCapsuleComponent());
}

void AP11Character::FindDefaultReferences()
{
	if (!DefaultContext)
	{
		const FString DefaultPath = "/Script/EnhancedInput.InputMappingContext'/Game/Characters/Input/IMC_Default.IMC_Default'";
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultContextFinder(*DefaultPath);
		if (DefaultContextFinder.Succeeded())
		{
			DefaultContext = DefaultContextFinder.Object;
		}
	}

	TMap<FString, UInputAction**> ActionsMap;
	ActionsMap.Add("Move",   &MoveAction);
	ActionsMap.Add("Look",   &LookAction);
	ActionsMap.Add("Jump",   &JumpAction);
	ActionsMap.Add("Crouch", &CrouchAction);
	ActionsMap.Add("Fire",   &FireAction);
	ActionsMap.Add("Aim",    &AimAction);
	ActionsMap.Add("Reload", &ReloadAction);
	ActionsMap.Add("Change", &ShiftCameraAction);
	for (auto& [ActionName, ActionReference] : ActionsMap)
	{
		if (*ActionReference)
		{
			continue;
		}
		const FString FullPath = "/Game/Characters/Input/Actions/IA_" + ActionName;
		ConstructorHelpers::FObjectFinder<UInputAction> ActionFinder(*FullPath);
		if (ActionFinder.Succeeded())
		{
			*ActionReference = ActionFinder.Object;
		}
	}

	TMap<FString, USoundBase**> SoundMap;
	SoundMap.Add("BlasterShoot", &FireSound);
	SoundMap.Add("OutOfAmmo", &NoAmmoSound);
	for (auto& [SoundName, SoundReference] : SoundMap)
	{
		if (*SoundReference)
		{
			continue;
		}
		const FString FullPath = "/Game/Project/PP11/Audio/MFX_" + SoundName;
		ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder(*FullPath);
		if (SoundFinder.Succeeded())
		{
			*SoundReference = SoundFinder.Object;
		}
	}
}

AP11HUD* AP11Character::GetPlayerHUD()
{
	if (!GetController() || !GetController()->IsLocalController())
	{
		return nullptr;
	}
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		return nullptr;
	}
	return Cast<AP11HUD>(PlayerController->GetHUD());
}

void AP11Character::OnRep_Health()
{
	OnHealthChanged.Broadcast(Health);
}

void AP11Character::OnRep_Ammo()
{
	OnAmmoChanged.Broadcast(Ammo);
}

void AP11Character::ReturnShootAbility(const EP11CharState StateBeforeShoot)
{
	CharState = StateBeforeShoot;
}

void AP11Character::UpdateMeshOnServer(const EP11PlayerSide NewSide)
{
	PlayerSide = NewSide;
	/** After changing PlayerSide, OnRep function will be called on the clients (but not on the server).
	  * We need to call OnRep here, so than changes are also applied on the server. */
	OnRep_PlayerSide();
}

void AP11Character::OnRep_PlayerSide()
{
	UpdateMaterials();
}

void AP11Character::UpdateMaterials() const
{
	const FLinearColor NewColor = (PlayerSide == EP11PlayerSide::Jedi) ? FLinearColor::White : FLinearColor::Red;
	
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;
	DynamicMaterials.Add(GetMesh()->CreateAndSetMaterialInstanceDynamic(1));
	DynamicMaterials.Add(GetMesh()->CreateAndSetMaterialInstanceDynamic(2));
	DynamicMaterials.Add(GetMesh()->CreateAndSetMaterialInstanceDynamic(3));
	
	for (const auto& DynamicMaterial : DynamicMaterials)
	{
		DynamicMaterial->SetVectorParameterValue("MainColor", NewColor);
		DynamicMaterial->SetVectorParameterValue("PaintColor", NewColor);
	}
}

void AP11Character::AddScore(const AController* Killer)
{
	if (AP11PlayerState* VictimPlayerState = GetPlayerState<AP11PlayerState>())
	{
		VictimPlayerState->AddDeath();
	}
	if (!Killer)
	{
		return;
	}
	if (AP11PlayerState* KillerPlayerState = Killer->GetPlayerState<AP11PlayerState>())
	{
		KillerPlayerState->AddKill();
	}
}
