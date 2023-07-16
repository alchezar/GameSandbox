// Copyright (C) 2023, IKinder

#include "P7/Public/Component/P7WallRunComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P7/Public/Player/P7Character.h"

UP7WallRunComponent::UP7WallRunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP7WallRunComponent::BeginPlay()
{
	Super::BeginPlay();
	InitCharacterReference();
	Player->MovementModeChangedDelegate.AddDynamic(this, &ThisClass::ManageTick);
	Player->OnActorHit.AddDynamic(this, &ThisClass::CheckForFloor);
}

void UP7WallRunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/** This tick event is only active when the player is wall-running or while they're in the air.
	  * We perform different actions depending on whether the player is already wall-running. */
	if (bWallRunning)
	{
		/* Perform traces in the direction of the current wall to determine if we're still on it. */
		DetectWall(WallSide);
	}
	else
	{
		/** If we are not wall-running, perform wall-jump updates (if wall jumping),
		  * then look for walls on either side of the player to begin a wall run on.
		  * We first check the right side of the player, and if no wall is detected, we then check the left side. */
		WallJumpTick(DeltaTime);
		DetectWall(EWS_Right);
		if (!bWallRunning)
		{
			DetectWall(EWS_Left);
		}
	}
	/* Perform all wall-run updates if we're still wall-running after all of the previous checks. */
	WallRunTick();
	/** Record the player's velocity at the end of the tick.
	  * This is done so that we can know how the player was moving during the tick immediately before they impacted a wall. */
	PreviousVelocity = Player->GetVelocity();
}

void UP7WallRunComponent::InitCharacterReference()
{
	/* Save a reference for the character and its movement component */
	Player = Cast<AP7Character>(GetOwner());
	if (!Player) return;
	MovementComponent = Player->GetCharacterMovement();
	check(Player);
	check(MovementComponent);
	/* Record information about the character, such as its original movement settings. */
	GatherPlayerInfo();	
	/** Calculate a value that will be used as the radius for traces later on.
	  * It's based on the capsule size to support characters of different sizes. */
	PrimaryTraceSize = CapsuleRadius * 0.85;
	/** If the player was spawned in the air (and is therefore in the Falling state), activate the component's tick.
	  * This is normally handled when the movement mode changes to the Falling state,
	  * but if the character is spawned while falling, it has to be handled separately. */
	if (MovementComponent->MovementMode == MOVE_Falling)
	{
		SetComponentTickEnabled(true);
	}
}

/** When the character's movement mode changes, enable or disable this component's tick depending on the mode.
  * When the character enters the Falling state, tick is enabled so that it can begin searching for walls to run on.
  * When any other mode is activated (except for Flying), tick is disabled.
  * Flying is handled differently because that's the mode that is utilized while running on a wall.
  * If Flying becomes active but the character isn't wall running, we disable the component's tick
  * because that suggests some other gameplay feature outside of wall running is using the Flying mode and we don't want to interfere with it.
  */
// ReSharper disable once CppParameterMayBeConstPtrOrRef
void UP7WallRunComponent::ManageTick(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	const EMovementMode MovementMode = MovementComponent->MovementMode;
	if (MovementMode == MOVE_Falling)
	{
		SetComponentTickEnabled(true);
		/* Clear any possible old data from the PreviousVelocity variable by re-initializing it to the current velocity. */
		PreviousVelocity = Character->GetVelocity();
		return;
	}
	if (MovementMode == MOVE_Flying)
	{
		if (bWallRunning) return;
	}
	if (IsComponentTickEnabled())
	{
		SetComponentTickEnabled(false);
	}
	/* If we are currently wall jumping, end the jump.
	 * (The change in movement mode suggests the player may have hit the ground after jumping.) */
	if (bWallJumping)
	{
		StopWallJump();
	}
}

/** When a hit event occurs on a wall-running character, check if the hit is a walkable surface and if it's below the character's feet.
  * If it is, end the wall run and land on the floor. */
void UP7WallRunComponent::CheckForFloor(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!MovementComponent->IsWalkable(Hit)) return;
	
	if (bWallRunning)
	{
		const float HalfHeight = Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight_WithoutHemisphere();
		const float CharacterKneeHeight = Player->GetActorLocation().Z - HalfHeight;
		if (Hit.ImpactPoint.Z > CharacterKneeHeight) return;
		StopWallRun(MOVE_Walking, 0.f);
		return;
	}
	
	/** If the character landed on a floor and wall running is temporarily disabled,
	  * immediately re-enable it and stop the timer that would have done so after a delay.
	  * Landing on the floor should immediately clear any restriction on wall running
	  * and allow the player to immediately begin wall running again. */
	GetWorld()->GetTimerManager().ClearTimer(DisableWallRunTimer);
	AllowWallRunning();
}

void UP7WallRunComponent::StartDurationCountdown()
{
	/** Called when a wall run begins if a max duration is specified for the wall run.
	  * Starts a timer which waits for the desired amount of time, then stops the wall run.
	  * Wall running is then disabled for a short time so that the player can't immediately start a new wall run. */
	GetWorld()->GetTimerManager().SetTimer(DurationCountDownTimer, this, &ThisClass::WallRunTimeout, WallRunDurationLimit);
}

void UP7WallRunComponent::WallRunTimeout()
{
	StopWallRun(MOVE_Falling, 2.f);
}

/* Temporarily disallow wall running for a specified amount of time. */
void UP7WallRunComponent::TemporarilyDisableWallRunning(const float DisableTime)
{
	bWallRunningAvailable = false;
	GetWorld()->GetTimerManager().SetTimer(DisableWallRunTimer, this, &ThisClass::AllowWallRunning, DisableTime);
}

void UP7WallRunComponent::AllowWallRunning()
{
	bWallRunningAvailable = true;
}

void UP7WallRunComponent::StartMonitoringSpeed()
{
	/** Starts a timer that monitors the character's X and Y velocity at set intervals during a wall run.
	  * This is a failsafe to detect if the player gets stuck during a wall run
	  * and will cancel the run if they're stuck for two consecutive intervals.
	  * The SpeedTooLow boolean is set to false when monitoring begins and will be set to true if the test fails. */
	bSpeedTooLow = false;
	GetWorld()->GetTimerManager().SetTimer(SpeedMonitoringTimer, this, &ThisClass::MonitorSpeed, 0.2f, true);
}

void UP7WallRunComponent::MonitorSpeed()
{
	/** Checks if the character's X/Y velocity is at least 25% of the desired run speed.
	  * If it is lower than that, that suggests the wall run is being blocked by something.
	  * The first time this happens, we only set SpeedTooLow to true to mark that it has failed the test once -
	  * but we don't want to do anything about it yet in case it's just a momentary drop in velocity.
	  * If the test fails again in the next interval, then we stop the wall run.
	  * If the test does not fail two times consecutively, we set SpeedTooLow back to false so the process can start over. */
	if (Player->GetVelocity().Projection().Length() < WallRunSpeed * 0.25f)
	{
		if (bSpeedTooLow)
		{
			StopWallRun(MOVE_Falling, 1.f);
			return;
		}
		bSpeedTooLow = true;
		return;
	}
	if (bSpeedTooLow) bSpeedTooLow = false;
}

/* Called when a wall run ends. This just stops the timer that was started by the StartMonitoringSpeed event. */
void UP7WallRunComponent::StopMonitoringSpeed()
{
	GetWorld()->GetTimerManager().ClearTimer(SpeedMonitoringTimer);
}

void UP7WallRunComponent::DelayedAirControl()
{
	/** This delay allows the wall jump direction and velocity to establish itself before allowing the player to override the direction of the jump,
	 * guaranteeing consistency with the initial jump direction/velocity regardless of player input. */
	GetWorld()->GetTimerManager().SetTimer(WallJumpAirControlTimer, this, &ThisClass::EnableWallJumpAirControl, 0.35f);
}

void UP7WallRunComponent::EnableWallJumpAirControl()
{
	/** Acceleration is scaled up along with the WallJumpAirControl parameter
	 * because setting AirControl on its own wasn't providing enough control during the jump. */
	MovementComponent->AirControl = WallJumpAirControl;
	MovementComponent->MaxAcceleration = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 1.f), FVector2D(2048.f, 6000.f), WallJumpAirControl);
}

void UP7WallRunComponent::DetectWall(EWallSide Side)
{
	/** Primary trace - look for a wall to the left or right of the character (depending on which side was provided when the function was called).
	 * The trace begins at the character's location and ends a short distance to the character's left or right. */
	FHitResult PrimaryHit;
	FVector Start = Player->GetActorLocation();
	/* Move the trace forward slightly. This allows detecting walls at sharper (closer to perpendicular) angles. */
	Start += CapsuleRadius * 0.05f * Player->GetActorForwardVector();
	/* Perform a longer trace if we're already running on a wall to better detect corners */
	const float TraceLength = bWallRunning ? 2.f : 0.3f;
	/* Modify the direction of the trace depending on which side has been provided */
	const float TraceDirection = Side == EWS_Right ? 1.f : -1.f;
	/* Trace length is based on the character's capsule size in order to support differently sized characters */
	FVector End = Start + Player->GetActorRightVector() * CapsuleRadius * TraceLength * TraceDirection;
	FCollisionShape Sphere;
	Sphere.SetSphere(PrimaryTraceSize);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Player);
	GetWorld()->SweepSingleByChannel(PrimaryHit, Start, End, FQuat::Identity, ECC_Visibility, Sphere, QueryParams);
	if (!PrimaryHit.bBlockingHit || !GetIsWallRunnable(PrimaryHit.GetActor()))
	{
		/** If no eligible hit was detected and the character is currently wall running,
		  * it means the player has lost contact with the wall (most likely from running off the edge). Stop the wall run immediately. */
		if (bWallRunning) StopWallRun(MOVE_Falling, 0.f);
		return;
	}
	/** Record the impact normal as the PrimaryTraceNormal and as the AverageWallNormal, the latter of which may be modified in future steps.
	  * We also record the location of the wall for future reference. */
	PrimaryTraceNormal  = PrimaryHit.ImpactNormal;
	AverageWallNormal  = PrimaryTraceNormal;
	CurrentWallLocation = PrimaryHit.ImpactPoint;
	/** Find how much space is between the wall and the character's capsule.
	  * We find the distance between the start and impact locations of the trace, then subtract the character's capsule radius.
	  * This is later used in the HugWall function. */
	DistanceFromWall = FVector::Dist(PrimaryHit.TraceStart, CurrentWallLocation) - CapsuleRadius;

	/** Secondary trace - Looks for a wall a short distance in front of the player to identify any upcoming turns/corners.
	  * The positions of this trace are based on information gathered from the previous trace -
	  * it uses the impact point and wall normal from the previous trace to start a short distance forward and away from the wall,
	  * then traces in the reverse direction of that wall's normal (towards the wall).
	  * The end position is moved slightly forward to better detect inside corners. */
	constexpr bool bDrawAllDebugs = true;
	FHitResult SecondaryHit;
	FWallRunTraceInfo SecondaryTraceInfo = FWallRunTraceInfo(1.5f, -0.9f, 0.75f, -1.5f, 0.f);
	LineTraceRelativeToCapsuleAndWall(PrimaryTraceNormal, CurrentWallLocation, SecondaryTraceInfo, Side, SecondaryHit, bDrawAllDebugs);
	if (SecondaryHit.bBlockingHit)
	{
		/** If a hit was detected, make sure the hit wall is eligible for wall running. If the wall is not eligible - do nothing,
		  * when/if the player reaches this wall, it will be detected by the primary trace and then the wall run will end. */
		if (!GetIsWallRunnable(SecondaryHit.GetActor())) return;
		/** If we are already running on a wall, check the difference in angle between the Primary trace and this one.
		  * If it's too sharp of a corner, we may need to end the wall run. However, before doing so, we will perform a backup trace to be sure. */
		const bool bWithinLimits = CheckCornerAngle(PrimaryTraceNormal, SecondaryHit.ImpactNormal);
		if (!bWallRunning || bWallRunning && bWithinLimits)
		{
			/** If the angle is within limits or we have not yet started a wall run, record the normal as the SecondaryTraceNormal,
			  * then add it to (and normalize) the AverageWallNormal.
			  * This average is the combined normal of the two traces that will control the direction of the wall run. */
			SecondaryTraceNormal = SecondaryHit.ImpactNormal;
			AverageWallNormal = (AverageWallNormal + SecondaryTraceNormal).GetSafeNormal(UE_KINDA_SMALL_NUMBER);
		}
		else if(bWallRunning && !bWithinLimits)
		{
			// /** Perform a backup trace if the initial secondary trace detected a corner that was outside the desired limits.
			//   * This backup trace acts as a failsafe to protect against small gaps/cracks/lips in the wall's geometry.
			//   * To identify if the first trace hit a small gap that should be ignored, we perform this trace a small distance in front of the first one.
			//   * If this trace finds a surface that has an appropriate angle, we throw out the result of the first trace.
			//   * If this one also detects a surface with too sharp of an angle (or if it doesn't detect a surface at all),
			//   * we proceed with ending the wall run. */
			FHitResult BackupHit;
			FWallRunTraceInfo BackupTraceInfo = FWallRunTraceInfo(1.5f, -1.f, 0.75f, -1.8f, 0.f);
			LineTraceRelativeToCapsuleAndWall(PrimaryTraceNormal, CurrentWallLocation, BackupTraceInfo, Side, BackupHit, bDrawAllDebugs);
			/** If a hit was detected, make sure the hit wall is eligible for wall running.
			  * If there was no hit or  the wall is not eligible,
			  * we continue with the result of the initial secondary trace - the angle was too sharp, therefore we end the wall run.
			  * Determine if the surface hit by this trace is within the desired angle limits.*/
			if (!BackupHit.bBlockingHit || !GetIsWallRunnable(BackupHit.GetActor()) || !CheckCornerAngle(PrimaryTraceNormal, BackupHit.ImpactNormal))
			{
				StopWallRun(MOVE_Falling, 1.f);
				return;
			}
			/** If the angle is within limits, record the normal as the SecondaryTraceNormal, then add it to (and normalize) the AverageWallNormal.
			 * This average is the combined normal of the two traces that will control the direction of the wall run. */
			SecondaryTraceNormal = BackupHit.ImpactNormal;
			AverageWallNormal = (AverageWallNormal + SecondaryTraceNormal).GetSafeNormal(UE_KINDA_SMALL_NUMBER);
		}
	}
	else /* Means !SecondaryHit.bBlockingHit */
	{
		/** Backup / Corner trace - If the Secondary trace did not detect a surface,
		  * this trace checks for the possibility of an extremely sharp outside corner (~90 degrees).
		  * To detect sharp corners, the trace starts slightly past where the corner would be, then traces sharply backwards. */
		FHitResult SharpHit;
		FWallRunTraceInfo SharpTraceInfo = FWallRunTraceInfo(0.f, -1.5f, 0.f, 2.f, -0.3f);
		LineTraceRelativeToCapsuleAndWall(PrimaryTraceNormal, CurrentWallLocation, SharpTraceInfo, Side, SharpHit, bDrawAllDebugs);
		/** If a hit was detected, make sure the hit wall is eligible for wall running. If the wall is not eligible -do nothing,
		  * when/if the player reaches this wall, it will be detected by the primary trace and then the wall run will end. */
		if (SharpHit.bBlockingHit && GetIsWallRunnable(SharpHit.GetActor()))
		{
			/** If we're already wall running, determine if the surface hit by this trace is within the desired angle limits.
			  * If it is not within limits, we end the wall run. If we are not yet wall running, continue recording information about the wall. */
			if (bWallRunning && !CheckCornerAngle(PrimaryTraceNormal, SharpHit.ImpactNormal))
			{
				StopWallRun(MOVE_Falling, 1.f);
			}
			/** Record the normal as the SecondaryTraceNormal, then add it to (and normalize) the AverageWallNormal,
			 * which is now the average surface normal of the two traces that will control the direction of the wall run. */
			SecondaryTraceNormal = SharpHit.ImpactNormal;
			AverageWallNormal = (AverageWallNormal + SecondaryTraceNormal).GetSafeNormal(UE_KINDA_SMALL_NUMBER);
		}
		return;
	}
	/** If we aren't already running on a wall, check if there is enough vertical space on the wall to support wall running.
	  * If so, begin the wall run. */
	if (bWallRunning || !BottomHeightCheck(Side) || !TopHeightCheck()) return;
	StartWallRun(Side);
}

bool UP7WallRunComponent::GetIsWallRunnable(const AActor* Wall) const
{
	/** We first check if the actor is valid to avoid any output errors if this check is being run on brush geometry
	  * (which doesn't return an actor object and doesn't support actor tags).
	  * If it's not valid and a tag is required for wall running, we return false since brush geometry will never be tagged. */
	if (!Wall) return !bRequiresTag;
	/* Disallows wall running if the actor has a specific tag. */
	if (Wall->ActorHasTag(TagForPreventingWallRun)) return false;
	/* If "RequiresTag" is true, only allow wall running if the actor has the required tag. */
	if (!bRequiresTag) return true;
	return Wall->ActorHasTag(TagForAllowingWallRun);
}

bool UP7WallRunComponent::TopHeightCheck()
{
	/** This function identifies if there is enough space on the wall near the top of the player's capsule for the character to latch on to.
	  * If there isn't enough room, we either:
	  * 1. Prevent the wall run if one hasn't already begun
	  * 2. If we are already wall-running, prevent the player from running up and off of the wall's top ledge
	  * (this could occur when the wall-run is arcing upwards, or if the ledge angles downward into the wall-run's path)
	  * We start with a simple line trace because it's the least prone to giving false positives.
	  * The trace starts at the desired "max height" that the player's capsule should be relative to the top ledge of a wall
	  * (i.e. if the trace doesn't hit the wall, it suggests the player has gone too high), then moves towards the wall a short distance. */
	const FVector Start = Player->GetActorLocation() + Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.65f;
	FVector End = AverageWallNormal * CapsuleRadius * -1.7f;
	FHitResult TopHitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(TopHitResult, Start, End, ECC_Visibility, Params);
	/* Perform different actions depending on whether the trace finds an eligible wall. */
	if (TopHitResult.bBlockingHit && GetIsWallRunnable(TopHitResult.GetActor()))
	{
		/** If the trace(s) find that the player is at a reasonable height relative to the wall,
		  * record the player's z-location as the last known safe height and perform no further action. */
		LastKnownSafeHeight = Player->GetActorLocation().Z;
		return true;
	}
	/** In the event that the line trace fails to detect a wall, we perform a backup capsule trace.
	  * The additional size of this capsule trace is more likely to find where the top ledge of the wall is,
	  * but that also means it's more likely to find false positives, which is why this is only done if the first trace fails. */
	End = Start + AverageWallNormal * CapsuleRadius * -1.1f;
	FHitResult BackupHitResult;
	FCollisionShape Shape;
	Shape.SetSphere(CapsuleRadius * 0.6f);
	GetWorld()->SweepSingleByChannel(BackupHitResult, Start, End, FQuat::Identity, ECC_Visibility, Shape, Params);
	if (!BackupHitResult.bBlockingHit) return false;
	/* Perform different actions depending on whether the trace finds an eligible wall. */
	if (GetIsWallRunnable(BackupHitResult.GetActor()))
	{
		if (Start.Z < BackupHitResult.ImpactPoint.Z)
		{
			LastKnownSafeHeight = Player->GetActorLocation().Z;
			return true;
		}
		/** If the impact point is below the trace's start location (indicating that the player is higher than desired),
		  * and the player is currently wall-running, lower the player by the difference (how high above the wall they are)
		  * to place them at the desired max height for the current wall. */
		if (bWallRunning)
		{
			FVector NewLocation = Player->GetActorLocation();
			NewLocation.Z -= Start.Z - BackupHitResult.ImpactPoint.Z;
			Player->SetActorLocation(NewLocation, true);
			LastKnownSafeHeight = NewLocation.Z;
			return true;
		}
		return false;
	}
	if (!bWallRunning) return false;
	/* If the height check fails and the player is already wall-running, move the character down to the last recorded safe height. */
	FVector NewLocation = Player->GetActorLocation();
	NewLocation.Z = LastKnownSafeHeight;
	Player->SetActorLocation(NewLocation, true);
	return false;
}

bool UP7WallRunComponent::BottomHeightCheck(EWallSide Side)
{
	/** Check if there's a valid wall near the character's feet. The trace begins near the bottom of the capsule,
	  * then moves a short distance towards the desired side of the character.
	  * The trace lengths and radius are based on the character's capsule size to support characters of different sizes.
	  * This is done to prevent the character from continuing to wall run if the bottom half of their body is hanging off the bottom of a wall. */
	const FVector Start = Player->GetActorLocation() - FVector(0.f, 0.f, Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	const FVector Direction = Side == EWS_Right ? Player->GetActorRightVector() : Player->GetActorRightVector() * -1.f;
	const FVector End = Start + Direction * CapsuleRadius * 2.f;
	FHitResult BottomHitResult;
	FCollisionShape Shape;
	Shape.SetSphere(CapsuleRadius * 0.5f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	GetWorld()->SweepSingleByChannel(BottomHitResult, Start, End, FQuat::Identity, ECC_Visibility, Shape, Params);
	if (!BottomHitResult.bBlockingHit)
	{
		/* If a valid wall is not detected, return false and stop the wall run if one is active. */
		if (bWallRunning) StopWallRun(MOVE_Falling, 0.f);
		return false;
	}
	/* If a valid wall is detected, return True and take no further action. */
	return true;
	
}
	/** This function takes in information about the wall and desired values relevant to the trace, then performs a line trace.
	 * All values are based on the character's capsule size to support differently sized characters. */
bool UP7WallRunComponent::LineTraceRelativeToCapsuleAndWall(const FVector& WallNormalCurrent, const FVector& WallLocationCurrent, const FWallRunTraceInfo& TraceInfo, const EWallSide& Side, FHitResult& HitResult, const bool bShowDebug) const
{
	/* Finds the "forward" direction based on the wall's normal and side */
	const FVector SideVector = FVector(0.f, 0.f, Side == EWS_Right ? 1.f : -1.f);
	const FVector ForwardDirection = FVector::CrossProduct(WallNormalCurrent, SideVector);
	/* Start position forwards offset (positive = backwards) */
	const float ForwardOffset = CapsuleRadius * TraceInfo.StartOffsetBackwards;
	/* Start position sideways offset (positive = away from wall) */
	const FVector SideOffset = WallNormalCurrent * CapsuleRadius * TraceInfo.StartOffsetAwayFromWall;
	/* Sets the trace's length before any end position offsets are applied. The value is negative so that the trace points towards the wall. */
	const FVector EndOffset = WallNormalCurrent * CapsuleRadius * TraceInfo.InitialTraceLength * -1.f;
	/* End position forwards offset (positive = backwards) */
	const FVector EndOffsetForward = ForwardDirection * CapsuleRadius * TraceInfo.EndOffsetBackwards;
	/* End position sideways offset (positive = away from wall) */
	const FVector EndOffsetSide = WallNormalCurrent * CapsuleRadius * TraceInfo.EndOffsetAwayFromWall;
	/* Combining/finalizing start position */
	const FVector Start = WallLocationCurrent + (ForwardDirection * ForwardOffset) + SideOffset;
	/* Combining/finalizing end position */
	const FVector End = Start + EndOffset + EndOffsetForward + EndOffsetSide;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	const bool bTrace = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);
	/* Allow display of the debug lines if desired */
	if (bShowDebug)
	{
		constexpr float LifeTime = 20.f;
		if (bTrace)
		{
			DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Red, false, LifeTime);
			DrawDebugLine(GetWorld(), HitResult.ImpactPoint, End, FColor::Green, false, LifeTime);
			DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.f, FColor::Red, false, LifeTime);
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, LifeTime);
		}
	}
	/* Return true and the Hit Result if the line trace hit a surface. Otherwise, return false. */
	return bTrace;
}

bool UP7WallRunComponent::CheckCornerAngle(const FVector& FirstWallNormal, const FVector& SecondWallNormal)
{
	const double CornerAngle = FMath::Acos(FVector2D::DotProduct(FVector2D(FirstWallNormal), FVector2D(SecondWallNormal)));
	/** Chop off the decimal to prevent slight inaccuracies from causing wallruns to end
	  * (for example, flat walls would sometimes report 0.02 instead of 0 - which would cause a wall run to end incorrectly
	  * if the max corner angle was set to 0).
	  * This does mean there's about 1 more degree being allowed than desired (if max angle is 45, this would allow 45.9 to proceed),
	  * but I think it's the most elegant solution. */
	CurrentCornerAngle = FMath::TruncToFloat(CornerAngle);
	/** Determines whether we're evaluating an inside or outside corner and returns the appropriate angle limit.
	  * We identify which way the wall is turning by getting the cross product of the two surface normals
	  * (negative z = turning left, positive z = turning right).
	  * Combining that with which side of the character the wall is on tells us if we're navigating an inside or outside corner. */
	const double Sign = FMath::Sign(FVector::CrossProduct(FirstWallNormal, SecondWallNormal).Z);
	const bool bOutsideCorner = (WallSide == EWS_Left && Sign < 0.0) || (WallSide == EWS_Right && Sign >= 0.0);
	const float CurrentAngleLimit = bOutsideCorner ? OutsideCornerLimit : InsideCornerLimit;
	/** Check how sharp the angle is between the two surface normals provided.
	  * Return true or false depending on whether or not the corner is within our limits. */
	return CurrentCornerAngle <= CurrentAngleLimit;
}

bool UP7WallRunComponent::StartWallRun(const EWallSide Side)
{
	/* Only start the wall run if it is allowed. */
	if (!bWallRunningAvailable) return false;
	/** Only allow the wall run to begin if:
	 * - The player character was moving in the direction of the wall during the previous tick
	 * (we use the previous tick's velocity because if the wall detection trace is triggered during the same tick
	 * that the character's capsule hits the wall, the current velocity reflects the post-impact velocity, which is not useful to us here)
	 * OR
	 * - The player is providing movement input in the direction of the wall
	 * If the result of the dot products are negative, the character (or its input vector) is moving towards the wall.
	 * If it's positive, they're moving away from the wall. */
	const FVector2D PreviousVelocity2D = FVector2D(PreviousVelocity.GetSafeNormal());
	const FVector2D LastInputVector2D  = FVector2D(MovementComponent->GetLastInputVector());
	const FVector2D AverageNormal2D    = FVector2D(AverageWallNormal);
	const bool bPlayerMoveToWall  = 0.0 > FVector2D::DotProduct(PreviousVelocity2D, AverageNormal2D);
	const bool bInputVectorToWall = 0.0 > FVector2D::DotProduct(LastInputVector2D, AverageNormal2D);
	if (!bPlayerMoveToWall && !bInputVectorToWall) return false;
	/* If currently wall jumping, end the jump. */
	if (bWallJumping) StopWallJump();
	/* Update values for tracking wall run state. */
	bWallRunning = true;
	WallSide = Side;
	/* Apply all the necessary movement settings for the wall run. */
	MovementComponent->MaxFlySpeed = WallRunSpeed;
	MovementComponent->bOrientRotationToMovement = false;
	MovementComponent->MaxAcceleration = WallRunAcceleration;
	MovementComponent->BrakingDecelerationFlying = WallRunBrakingDeceleration;
	MovementComponent->SetMovementMode(MOVE_Flying);
	/* Reset the float that tracks how long the current wall run has been active. This is used to control the arc of the wall run. */
	CurrentWallRunDuration = 0.f;
	/* Initialize the current rotation as the "PreviousRotation" to get rid of any outdated data from a past wall run. */
	PreviousRotation = Player->GetActorRotation();
	/* If we want the wall run to automatically end after a certain amount of time, start the countdown that will end it. */
	if (WallRunDurationLimit >= 0.f) StartDurationCountdown();
	/* Start a timer that tracks the player's speed at certain intervals to identify if the wall run has gotten stuck on something. */
	StartMonitoringSpeed();
	/** Clear any existing velocity on the character so that the wall run always behaves consistently
	  * (otherwise the run's distance/height could be affected by the velocity the character had when the run began).
	  * This also prevents old velocity values from negatively affecting the arc angle animation blend, which is based on velocity. */
	MovementComponent->Velocity = FVector::ZeroVector;
	return true;
} 

void UP7WallRunComponent::StopWallRun(const EMovementMode NewMovementMode, const float TempDisableWallRunSec)
{
	/* Set bWallRunning (state tracker) to false, and restore the character's original movement settings. */
	bWallRunning = false;
	MovementComponent->bOrientRotationToMovement = bDefaultOrientRotation;
	MovementComponent->MaxAcceleration = DefaultMaxAcceleration;
	MovementComponent->MaxFlySpeed = DefaultMaxFlySpeed;
	MovementComponent->BrakingDecelerationFlying = DefaultBrakingDeceleration;
	
	/** Set the character to use the desired movement mode. This is done because depending on how the wall run ended,
	  * we may want the player to enter either the Walking or Falling state. */
	MovementComponent->SetMovementMode(NewMovementMode);
	
	/* Cancel the max wall run duration countdown if it's currently active and stop monitoring the character's speed. */
	GetWorld()->GetTimerManager().ClearTimer(DurationCountDownTimer);
	StopMonitoringSpeed();

	/* Temporarily disable wall running if desired. */
	if (TempDisableWallRunSec > 0.f)
	{
		TemporarilyDisableWallRunning(TempDisableWallRunSec);
	}
}

void UP7WallRunComponent::WallRunTick()
{
	/* Only perform these updates if we're currently wall running. */
	if (!bWallRunning) return;
	/* Keep the character close to the wall (but not too close). */
	WallHug();
	/** Add movement input to the character in the direction of the desired wall run.
	  * We get the movement direction by finding the cross product of the AverageWallNormal and the up/down vectors
	  * (up or down depending on which side of the character the wall is on). */
	const FVector Direction =  AverageWallNormal.Cross(FVector(0.f, 0.f, WallSide == EWS_Right ? -1.f : 1.f));
	/** Add an arc to the wall run by adding a Z-component to the movement input. The Z-component is defined by taking the current wall run duration,
	  * mapping it to a range of 1 to -2 over a set amount of time, then multiplying it by the global ArcAmount scalar.
	  * The result is that the arc begins at 1 (upward), then smoothly transitions to -2 (downward) over time. */
	FVector ArcDirection = Direction;
	ArcDirection.Z = ArcAmount * FMath::GetMappedRangeValueClamped(FVector2D(0.f, 2.5f), FVector2D(1.f, -2.f), CurrentWallRunDuration);
	/** The Scale Value is set very high so that it dwarfs/overrides any manual input being applied by the player.
	  * This method may seem odd, but it's much simpler than having to add logic to the character BP to filter the player's movement inputs
	  * at the appropriate times (which would make merging the system into other characters more complicated). */
	Player->AddMovementInput(ArcDirection, 10000.f);
	/** Smoothly rotate the character so it faces the direction the wallrun is going.
	  * The RInterp Speed increases proportionally with the character's velocity so that higher speeds = faster turning.
	  * This makes high speed turns look nicer, and also helps the system track corners better at high speed. */
	const FRotator TargetRotation = Direction.ToOrientationRotator();
	const float DeltaSec = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f;
	const float InterpSpeed = Player->GetVelocity().Length() / 100.f;
	Player->SetActorRotation(FMath::RInterpTo(Player->GetActorRotation(), TargetRotation, DeltaSec, InterpSpeed));
	/* Track the amount of time the current wall run has been active by incrementing the float by the World Delta. */
	CurrentWallRunDuration += DeltaSec;
	/* Check if there is enough vertical space on the wall for the player to continue wall running.
	 * If the player is approaching the top ledge of a wall, this caps their vertical movement so they won't run up off the top of it.
	 * We then check if there's enough vertical space near the bottom of the character.
	 * If the bottom of the capsule starts to hang off the bottom of the wall, the run will be ended. */
	TopHeightCheck();
	BottomHeightCheck(WallSide);
	/* Update any values that are only used for animation purposes. */
	UpdateAnimationValues();
}

void UP7WallRunComponent::WallHug()
{
	/** Nudges the character towards or away from the wall with the goal of keeping 1 unit between the wall and the character.
	  * This is done for two reasons:
	  * 1. If the character gets too far away from the wall, the wall will be lost and the wall run will end
	  * 2. If the character gets too close to the wall, it can cause undesired changes in how the character moves
	  * (I think it's caused by the capsule bumping into the wall)
	  * The DistanceFromWall variable is recorded during the DetectWall function.
	  * It is the amount of space between the wall and the character's capsule.
	  * By subtracting it from 1, we find how far we'd need to move the character to preserve a 1 unit gap between the capsule and the wall. */
	Player->AddActorWorldOffset(AverageWallNormal * (1.f - DistanceFromWall), true);
}

bool UP7WallRunComponent::StartWallJump()
{
	SetComponentTickEnabled(true);
	// ManageTick(Player, MovementComponent->GetGroundMovementMode(), 0);
	/* If the player is not wall running, stop the function and return False to indicate no jump was performed. */
	if (!bWallRunning) return false;
	/* If the player is currently wall running, stop the wall run and proceed with the wall jump. */
	StopWallRun(MOVE_Falling, 0.1f);
	/** Perform the jump by launching the character with the desired direction/velocity.
	 * The direction is determined by taking the current AverageWallNormal
	 * (which points directly away from the wall) and rotating it based on the provided WallJumpAngle.
	 * The angle is mapped to a reverse range (from 90->0 to 0->90)
	 * because the provided angle assumes 0 = forward and 90 = perpendicular to the wall,
	 * but the wall normal we start with is already perpendicular to the wall.
	 * So if 90 degrees is provided, we do not rotate the vector.
	 * Z-velocity is provided separately as its own parameter. */
	const FVector2D InputRange  = FVector2D(90.f, 0.f);
	const FVector2D OutputRange = FVector2D(0.f, WallSide == EWS_Right ? 90.f : -90.f);
	const float WallRotationYaw = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, WallJumpAngle);
	const FRotator Rotation     = FRotator(0.f, WallRotationYaw, 0.f);
	const FVector RotatedVector = Rotation.RotateVector(AverageWallNormal);
	const FVector Velocity2D = RotatedVector * WallJumpHorizontalVelocity;
	Player->LaunchCharacter(FVector(Velocity2D.X, Velocity2D.Y, WallJumpVerticalVelocity), true, true);
	/* Record that we are now in the WallJumping state */
	bWallJumping = true;
	/** Disable OrientRotationToMovement so that we can manually control character rotation,
	  * then remove all air control. We then grant air control (if allowed via the parameter) after a short delay. */
	MovementComponent->bOrientRotationToMovement = false;
	MovementComponent->AirControl = 0.f;
	/* After a short delay, apply the necessary movement settings for the wall jump. */
	DelayedAirControl();
	return true;
}

void UP7WallRunComponent::StopWallJump()
{
	/* End the WallJumping state and reset the character's movement settings to their original values. */
	bWallJumping = false;
	MovementComponent->bOrientRotationToMovement = bDefaultOrientRotation;
	MovementComponent->MaxAcceleration = DefaultMaxAcceleration;
	MovementComponent->AirControl = DefaultAirControl;
	
	/* If the WallJumpAirControlTimer (which allows air control after a certain delay) is still counting down, stop it.
	 * This prevents the timer from erroneously applying WallJump movement settings after a jump has already ended. */
	GetWorld()->GetTimerManager().ClearTimer(WallJumpAirControlTimer);
}

void UP7WallRunComponent::WallJumpTick(const float DeltaTime) const
{
	/** If currently wall jumping, smoothly rotate the character to face the direction they are currently moving in.
	  * This is used instead of OrientRotationToMovement because OrientRotationToMovement rotates the character in the
	  * direction of movement input being applied to the character, and not the actual direction of the character's movement. */
	if (!bWallJumping) return;
	const FRotator CurrentRotation = Player->GetActorRotation();
	const FRotator TargetRotation = FRotator(0.f, Player->GetVelocity().Rotation().Yaw, 0.f);
	const FRotator InterpRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.f);
	Player->SetActorRotation(InterpRotation);
}

void UP7WallRunComponent::GatherPlayerInfo()
{
	CapsuleRadius = Player->GetCapsuleComponent()->GetScaledCapsuleRadius();
	DefaultMaxFlySpeed		   = MovementComponent->MaxFlySpeed;
	DefaultMaxAcceleration     = MovementComponent->MaxAcceleration;
	DefaultBrakingDeceleration = MovementComponent->BrakingDecelerationFlying;
	DefaultAirControl		   = MovementComponent->AirControl;
	bDefaultOrientRotation	   = MovementComponent->bOrientRotationToMovement;
}

void UP7WallRunComponent::UpdateAnimationValues()
{
	/** Gets the current angle of the wall run's arc. Finds the Dot product of the player's velocity and the same velocity with Z set to 0.
	  * Negative values get flipped to positive when doing this, so the final value is multiplied by the sign (1 or -1) of the original Z value. */
	const FVector VelocityDirection = Player->GetVelocity().GetSafeNormal();
	FVector Direction2D = VelocityDirection;
	Direction2D.Z = 0.f;
	const float AngleSign = FMath::Sign(VelocityDirection.Z);
	CurrentArcAngle = AngleSign * FMath::RadiansToDegrees(FMath::Acos(VelocityDirection.Dot(Direction2D)));
	/** Find how fast we're turning around an outside corner and map it to a range of 0-1.
	  * To do this, we get the delta of the character's current and previous rotation, map it to a range of -1 to 1,
	  * switch the sign depending on what side the wall is on (so that outside corners always result in a positive value),
	  * then clamp the value so it can't be below 0 because we aren't interested in the turn rate of inside corners.
	  * The turn rate float is then interpolated so that its value changes are smooth. */
	FRotator CornetDelta = (Player->GetActorRotation() - PreviousRotation);
	CornetDelta.Normalize();
	const float DeltaSec = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.05f;
	float CornerRange = FMath::GetMappedRangeValueClamped(FVector2D(-10.f, 10.f), FVector2D(-1.f, 1.f), (CornetDelta.Vector() / DeltaSec).Y);
	CornerRange *= WallSide == EWS_Right ? 1.f : -1.f;
	CornerRange = FMath::Max(0.f ,CornerRange);
	CurrentTurnRate = FMath::FInterpTo(CurrentTurnRate, CornerRange, DeltaSec, 10.f);
	PreviousRotation = Player->GetActorRotation();
}
