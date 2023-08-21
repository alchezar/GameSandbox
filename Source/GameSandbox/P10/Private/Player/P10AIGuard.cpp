// Copyright (C) 2023, IKinder

#include "P10/Public/Player/P10AIGuard.h"

#include "Components/WidgetComponent.h"
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
}

void AP10AIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP10AIGuard::OnSeePawnHandle(APawn* Pawn)
{
	if (AP10GameMode* GameMode = Cast<AP10GameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->CompleteMission(Pawn, false);
	}
	ChangeGuardState(EP10AIGuardState::Alert);
}

void AP10AIGuard::OnHearNoiseHandle(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	DrawDebugString(GetWorld(), Location, FString::Printf(TEXT("Hear")), nullptr, FColor::Blue, 2.f);

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
}
