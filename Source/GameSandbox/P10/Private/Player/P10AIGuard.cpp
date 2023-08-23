// Copyright (C) 2023, IKinder

#include "P10/Public/Player/P10AIGuard.h"

#include "AIController.h"
#include "Components/WidgetComponent.h"
#include "Engine/TargetPoint.h"
#include "Navigation/PathFollowingComponent.h"
#include "Net/UnrealNetwork.h"
#include "P10/Public/Game/P10GameMode.h"
#include "P10/Public/Player/P10Character.h"
#include "P10/Public/UI/P10GuardStateWidget.h"
#include "Perception/PawnSensingComponent.h"

AP10AIGuard::AP10AIGuard()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	StatusWidget = CreateDefaultSubobject<UWidgetComponent>("StatusWidgetComponent");
	StatusWidget->SetupAttachment(RootComponent);
	StatusWidget->SetWidgetSpace(EWidgetSpace::Screen);
	StatusWidget->SetDrawAtDesiredSize(true);
}

void AP10AIGuard::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensing->OnSeePawn.AddDynamic(this, &ThisClass::OnSeePawnHandle);
	PawnSensing->OnHearNoise.AddDynamic(this, &ThisClass::OnHearNoiseHandle);
}

void AP10AIGuard::BeginPlay()
{
	Super::BeginPlay();

	if (UP10GuardStateWidget* StateWidget = Cast<UP10GuardStateWidget>(StatusWidget->GetUserWidgetObject()))
	{
		StateWidget->SetGuard(this);
		ChangeGuardState(GuardState);
	}
	OriginalRotation = GetActorRotation();

	MoveToTarget();
}

void AP10AIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP10AIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, GuardState)
}

void AP10AIGuard::OnSeePawnHandle(APawn* Pawn)
{
	GetController()->StopMovement();
	if (AP10GameMode* GameMode = Cast<AP10GameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->CompleteMission(Pawn, false);
	}
	ChangeGuardState(EP10AIGuardState::Alert);
}

void AP10AIGuard::OnHearNoiseHandle(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	DrawDebugString(GetWorld(), Location, FString::Printf(TEXT("Hear")), nullptr, FColor::Blue, 2.f);

	GetController()->StopMovement();
	/* Remember original rotation only if there were no distractions before. */
	if (GuardState == EP10AIGuardState::Idle)
	{
		OriginalRotation = GetActorRotation();
	}	
	ChangeGuardState(EP10AIGuardState::Suspicious);
	TargetRotation = FRotator(0.f, FRotationMatrix::MakeFromX(Location - GetActorLocation()).Rotator().Yaw, 0.f);
	/* Instead of using EventTick, start this timer for smooth rotation. */
	GetWorld()->GetTimerManager().SetTimer(DistractionTimer, this, &ThisClass::OrientGuardHandle, GetWorld()->GetDeltaSeconds(), true);
	/* Timer to return to original rotation. */
	GetWorld()->GetTimerManager().SetTimer(ResetTimer, this, &ThisClass::ResetRotationHandle, 5.f, false);

}

void AP10AIGuard::OrientGuardHandle()
{
	/* After reaching the target rotation - make OriginalRotation the new target. */
	if (GetActorRotation().Equals(TargetRotation, 1.f))
	{
		GetWorld()->GetTimerManager().ClearTimer(DistractionTimer);
		TargetRotation = GetActorRotation();

		if (GuardState == EP10AIGuardState::Idle)
		{
			MoveToTarget();
		}
		return;
	}
	/* Interpolation for smooth rotation. */
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 10.f));
}

void AP10AIGuard::ResetRotationHandle()
{
	/* Change state back to idle. */
	ChangeGuardState(EP10AIGuardState::Idle);
	
	TargetRotation = OriginalRotation;
	/* Instead of using EventTick, start this timer for smooth rotation. */
	GetWorld()->GetTimerManager().SetTimer(DistractionTimer, this, &ThisClass::OrientGuardHandle, GetWorld()->GetDeltaSeconds(), true);
}

void AP10AIGuard::ChangeGuardState(const EP10AIGuardState NewState)
{
	if (GuardState == NewState) return;
	
	GuardState = NewState;
	OnStatusChanged.Broadcast(GuardState);
	OnRep_GuardState();
}

void AP10AIGuard::OnRep_GuardState()
{
	OnStatusChanged.Broadcast(GuardState);
}

void AP10AIGuard::MoveToTarget()
{
	if (!bPatrol || Targets.IsEmpty()) return;	
	AAIController* AIController = Cast<AAIController>(Controller);
	if (!AIController) return;

	/* Get the random goal from the targets array. */
	int32 NextElement = FMath::RandRange(0, Targets.Num() - 1);
	if (NextElement == CurrentTargetsElement || !Targets[NextElement])
	{
		NextElement = (NextElement + 1) % Targets.Num();
	}
	const AActor* NextGoal = Targets[NextElement];
	CurrentTargetsElement = NextElement;

	FAIMoveRequest Request;
	Request.SetGoalActor(NextGoal);
	Request.SetAcceptanceRadius(50.f);
	Request.SetUsePathfinding(true);
	Request.SetAllowPartialPath(true);
	
	AIController->MoveTo(Request);
}
