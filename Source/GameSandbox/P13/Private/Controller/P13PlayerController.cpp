// Copyright © 2024, IKinder

#include "P13/Public/Controller/P13PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameSandbox.h"
#include "NiagaraFunctionLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "P13/Public/Intearface/P13InputInterface.h"

AP13PlayerController::AP13PlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AP13PlayerController::BeginPlay()
{
	Super::BeginPlay();
	AddDefaultMappingContext();
}

void AP13PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto* InputComp = Cast<UEnhancedInputComponent>(InputComponent);
	if (!InputComp)
	{
		return;
	}
	InputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
	InputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);

	InputComp->BindAction(SetDestinationAction, ETriggerEvent::Started, this, &ThisClass::OnInputStarted);
	InputComp->BindAction(SetDestinationAction, ETriggerEvent::Triggered, this, &ThisClass::OnSetDestinationTriggered);
	InputComp->BindAction(SetDestinationAction, ETriggerEvent::Completed, this, &ThisClass::OnSetDestinationReleased);
	InputComp->BindAction(SetDestinationAction, ETriggerEvent::Canceled, this, &ThisClass::OnSetDestinationReleased);

	InputComp->BindAction(SprintAction, ETriggerEvent::Started, this, &ThisClass::SprintInput, true);
	InputComp->BindAction(SprintAction, ETriggerEvent::Completed, this, &ThisClass::SprintInput, false);

	InputComp->BindAction(AimAction, ETriggerEvent::Started, this, &ThisClass::AimInput, true);
	InputComp->BindAction(AimAction, ETriggerEvent::Completed, this, &ThisClass::AimInput, false);

	InputComp->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ThisClass::ZoomInput);
}

void AP13PlayerController::OnInputStarted() 
{
	StopMovement();
}

void AP13PlayerController::OnSetDestinationTriggered() 
{
	/* We flag that the input is being pressed. */
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	/* We look for the location in the world where the player has pressed the input. */
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_CURSOR, true, Hit);

	/* If we hit a surface, cache the location. */
	if (Hit.bBlockingHit)
	{
		CachedDestination = Hit.Location;
	}
	
	/* Move towards mouse pointer. */
	ACharacter* OurChar = GetCharacter();
	if (!OurChar)
	{
		return;	
	}
	const FVector WorldDirection = (CachedDestination - OurChar->GetActorLocation()).GetSafeNormal();
	OurChar->AddMovementInput(WorldDirection, 1.0, false);

	// const FRotator MeshOldRotation = OurChar->GetMesh()->GetComponentRotation();
	// const FRotator MeshNewRotation = OurChar->GetCharacterMovement()->Velocity.GetSafeNormal().Rotation() - FRotator(0.f, 90.f, 0.f);
	// const FRotator InterpRotation = FMath::RInterpTo(MeshOldRotation, MeshNewRotation, GetWorld()->GetDeltaSeconds(), 2.f);
	// OurChar->GetMesh()->SetWorldRotation(InterpRotation);

	if (IP13InputInterface* InputInterface = Cast<IP13InputInterface>(GetPawn()))
	{
		const FVector ToCursorDirection = OurChar->GetCharacterMovement()->Velocity.GetSafeNormal();
		InputInterface->RotateTowardMovement(ToCursorDirection);
	}

#if WITH_EDITOR
	DrawDebugDirectionalArrow(GetWorld(), OurChar->GetActorLocation(), OurChar->GetActorLocation() + OurChar->GetCharacterMovement()->Velocity / 3.f, 50.f, FColor::Red);
	if (Hit.bBlockingHit)
	{
		DrawDebugPoint(GetWorld(), Hit.Location + FVector(0.f, 0.f, 20.f), 20.f, FColor::Red);
	}
#endif // WITH_EDITOR
}

void AP13PlayerController::OnSetDestinationReleased() 
{
	/* If it was a short press. */
	if (FollowTime <= ShortPressThreshold)
	{
		/* We move there and spawn some particles. */
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);

		if (const APawn* OurPawn = GetPawn())
		{
			const FVector DirectionToCursor = (CachedDestination - OurPawn->GetActorLocation()).GetSafeNormal2D();
			
			FTimerDelegate RotationTickDelegate;
			RotationTickDelegate.BindUObject(this, &ThisClass::RotateTowardCursorSmoothly, DirectionToCursor);
			GetWorld()->GetTimerManager().SetTimer(RotationTickTimer, RotationTickDelegate, GetWorld()->GetDeltaSeconds(), true);

			FTimerDelegate RotationDelegate;
			RotationDelegate.BindLambda([&]() { GetWorld()->GetTimerManager().ClearTimer(RotationTickTimer); });
			GetWorld()->GetTimerManager().SetTimer(RotationTimer, RotationDelegate, 2.f, false);
		}
	}

	FollowTime = 0.f;
}


void AP13PlayerController::MoveInput(const FInputActionValue& Value) 
{
	if (IP13InputInterface* InputInterface = Cast<IP13InputInterface>(GetPawn()))
	{
		InputInterface->MoveInput(Value.Get<FVector2D>());
	}
}

void AP13PlayerController::LookInput(const FInputActionValue& Value) 
{

}

void AP13PlayerController::SprintInput(const bool bStart)
{
	if (IP13InputInterface* InputInterface = Cast<IP13InputInterface>(GetPawn()))
	{
		InputInterface->SprintInput(bStart);
	}
}

void AP13PlayerController::AimInput(const bool bStart)
{
	if (IP13InputInterface* InputInterface = Cast<IP13InputInterface>(GetPawn()))
	{
		InputInterface->AimInput(bStart);
	}
}

void AP13PlayerController::ZoomInput(const FInputActionValue& Value)
{
	if (IP13InputInterface* InputInterface = Cast<IP13InputInterface>(GetPawn()))
    {
    	InputInterface->ZoomInput(Value.Get<float>());
    }
}

void AP13PlayerController::AddDefaultMappingContext()
{
	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!Subsystem)
	{
		return;
	}
	Subsystem->AddMappingContext(DefaultContext, 0);
}

void AP13PlayerController::RotateTowardCursorSmoothly(const FVector Direction)
{
	if (IP13InputInterface* InputInterface = Cast<IP13InputInterface>(GetPawn()))
	{
		InputInterface->RotateTowardMovement(Direction);
	}
}
