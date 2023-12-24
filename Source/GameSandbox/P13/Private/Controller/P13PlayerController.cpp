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

	const FRotator MeshRotation = OurChar->GetCharacterMovement()->Velocity.GetSafeNormal().Rotation() - FRotator(0.f, 90.f, 0.f);
	OurChar->GetMesh()->SetWorldRotation(MeshRotation);

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
	}

	FollowTime = 0.f;
}


void AP13PlayerController::MoveInput(const FInputActionValue& Value) 
{
	const FVector2D MoveVector = Value.Get<FVector2D>();
	APawn* OurPawn = GetPawn();
	if (!OurPawn)
	{
		return;
	}
	const FVector ForwardDirection = OurPawn->GetActorForwardVector();
	const FVector RightDirection = OurPawn->GetActorRightVector();
	
	OurPawn->AddMovementInput(ForwardDirection, MoveVector.Y);
	OurPawn->AddMovementInput(RightDirection, MoveVector.X);

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_CURSOR, true, Hit);
	if (Hit.bBlockingHit)
	{
		FVector LookAtDirection = (Hit.Location - OurPawn->GetActorLocation()).GetSafeNormal2D();
		FRotator LookAtRotation = LookAtDirection.Rotation() - FRotator(0.f, 90.f, 0.f);

		ACharacter* OurChar = Cast<ACharacter>(OurPawn);
		OurChar->GetMesh()->SetWorldRotation(LookAtRotation);
	}
}

void AP13PlayerController::LookInput(const FInputActionValue& Value) 
{

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
