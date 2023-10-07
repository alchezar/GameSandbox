// Copyright (C) 2023, IKinder

#include "P12/Public/Player/P12SpiderPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P12/Public/Component/P12SpiderMovementComponent.h"

AP12SpiderPawn::AP12SpiderPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereRootComponent = CreateDefaultSubobject<USphereComponent>("SphereRootComponent");
	SetRootComponent(SphereRootComponent);
	SphereRootComponent->SetSphereRadius(CollisionSphereRadius);
	SphereRootComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UP12SpiderMovementComponent>("SpiderMovementComponent");
	MovementComponent->SetUpdatedComponent(SphereRootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("CameraSpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SpiderSkeletalMeshComponent");
	SkeletalMeshComponent->SetupAttachment(RootComponent);

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("DirectionArrowComponent");
	ArrowComponent->SetupAttachment(GetRootComponent());
#endif
}

void AP12SpiderPawn::BeginPlay()
{
	Super::BeginPlay();
	GetSubsystem();
}

void AP12SpiderPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotatePawnToMovement(DeltaTime);

	constexpr float IKOffsetInterp = 10.f;
	IKRightFrontFootOffset = FMath::FInterpTo(IKRightFrontFootOffset, GetIKSocketOffset(RightFrontFootSocketName), DeltaTime, IKOffsetInterp);
	IKRightRearFootOffset  = FMath::FInterpTo(IKRightRearFootOffset,  GetIKSocketOffset(RightRearFootSocketName),  DeltaTime, IKOffsetInterp);
	IKLeftFrontFootOffset  = FMath::FInterpTo(IKLeftFrontFootOffset,  GetIKSocketOffset(LeftFrontFootSocketName),  DeltaTime, IKOffsetInterp);
	IKLeftRearFootOffset   = FMath::FInterpTo(IKLeftRearFootOffset,   GetIKSocketOffset(LeftRearFootSocketName),   DeltaTime, IKOffsetInterp);
}

void AP12SpiderPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!InputComp)
	{
		return;
	}
	InputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
	InputComp->BindAction(MoveAction, ETriggerEvent::Completed, this, &ThisClass::MoveInput);
	InputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);
	InputComp->BindAction(LookAction, ETriggerEvent::Completed, this, &ThisClass::LookInput);
	InputComp->BindAction(JumpAction, ETriggerEvent::Started,   this, &ThisClass::JumpInput);
}

void AP12SpiderPawn::GetSubsystem() const
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

void AP12SpiderPawn::MoveInput(const FInputActionValue& Value) 
{
	if (!Controller)
	{
		return;
	}
	InputForward = Value.Get<FVector2D>().X;
	InputRight   = Value.Get<FVector2D>().Y;

	const FRotator YawRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector RightDirection   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(ForwardDirection, InputForward);
	AddMovementInput(RightDirection, InputRight);
}

void AP12SpiderPawn::LookInput(const FInputActionValue& Value) 
{
	if (!Controller)
	{
		return;
	}
	FVector2D LookVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void AP12SpiderPawn::JumpInput() 
{
	check(MovementComponent->IsA<UP12SpiderMovementComponent>())
	UP12SpiderMovementComponent* SpiderMovement = StaticCast<UP12SpiderMovementComponent*>(MovementComponent);
	SpiderMovement->JumpStart();
}

void AP12SpiderPawn::RotatePawnToMovement(const float DeltaTime)
{
	if (FMath::IsNearlyZero(GetVelocity().Length()))
	{
		return;
	}
	const FRotator TargetRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	SphereRootComponent->SetWorldRotation(FMath::RInterpTo(SphereRootComponent->GetComponentRotation(), TargetRotation, DeltaTime, RotationInterpolationSpeed));
}

float AP12SpiderPawn::GetIKSocketOffset(const FName& SocketName)
{
	const FVector SocketLocation = SkeletalMeshComponent->GetSocketLocation(SocketName);
	const FVector TraceStart = FVector(SocketLocation.X, SocketLocation.Y, GetActorLocation().Z);
	const FVector TraceEnd = TraceStart - FVector::UpVector * GetActorScale3D().Z * CollisionSphereRadius * 2.f;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.bTraceComplex = true;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);
	DrawTraceDebug(HitResult);
	if (HitResult.bBlockingHit)
	{
		return ((HitResult.TraceEnd - HitResult.Location) / GetActorScale3D()).Z + CollisionSphereRadius;  
	}
	return 0.f;
}

void AP12SpiderPawn::DrawTraceDebug(const FHitResult& HitResult)
{
	if (!HitResult.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), HitResult.TraceStart, HitResult.TraceEnd, FColor::Green);
		return;
	}
	DrawDebugLine(GetWorld(), HitResult.TraceStart, HitResult.ImpactPoint, FColor::Red);
	DrawDebugLine(GetWorld(), HitResult.ImpactPoint, HitResult.TraceEnd, FColor::Green);
	DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.f, FColor::Red);
}
