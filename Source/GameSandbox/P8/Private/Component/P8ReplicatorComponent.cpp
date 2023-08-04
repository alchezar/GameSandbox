// Copyright (C) 2023, IKinder

#include "P8/Public/Component/P8ReplicatorComponent.h"
#include "Net/UnrealNetwork.h"
#include "P8/Public/Component/P8MovementComponent.h"

UP8ReplicatorComponent::UP8ReplicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UP8ReplicatorComponent::BeginPlay()
{
	Super::BeginPlay();
	check(MoveComp)
}

void UP8ReplicatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FP8Move LastMove = MoveComp->GetLastMove();
	/* We are the client */
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		UnacknowledgeMoves.Add(LastMove);
		Server_SendMove(LastMove);
	}
	/* We are the server and in control of the pawn */
	if (GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		// Server_SendMove(CurrentMove);
		UpdateServerState(LastMove);
	}
	/* Client prediction */
	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		ClientTick(DeltaTime);
	}
	DrawDebugString(GetWorld(), FVector(0.f, 0.f, 100.f), UEnum::GetValueAsString(GetOwnerRole()).RightChop(5), GetOwner(), FColor::White, 0.f);
}

void UP8ReplicatorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, ServerState);
}

void UP8ReplicatorComponent::SetMoveComp(UP8MovementComponent* MoveCompRef)
{
	MoveComp = MoveCompRef;
}

void UP8ReplicatorComponent::ClearAcknowledgeMoves(const FP8Move LastMove)
{
	TArray<FP8Move> NewMoves;
	for (const FP8Move& UnacknowledgeMove : UnacknowledgeMoves)
	{
		if (UnacknowledgeMove.Time > LastMove.Time)
		{
			NewMoves.Add(UnacknowledgeMove);
		}
	}
	UnacknowledgeMoves = NewMoves;
}

void UP8ReplicatorComponent::Server_SendMove_Implementation(FP8Move ServerMove)
{
	MoveComp->SimulateMove(ServerMove);
	UpdateServerState(ServerMove);
}
bool UP8ReplicatorComponent::Server_SendMove_Validate(const FP8Move ServerMove)
{
	return FMath::Abs(ServerMove.MoveAlpha) <= 1.f && FMath::Abs(ServerMove.TurnAlpha) <= 1.f;
}

void UP8ReplicatorComponent::UpdateServerState(const FP8Move LastMove)
{
	ServerState.LastMove = LastMove;
	ServerState.Transform = GetOwner()->GetActorTransform();
	ServerState.Velocity = MoveComp->GetVelocity();
}

void UP8ReplicatorComponent::OnRep_ServerState()
{
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		GetOwner()->SetActorTransform(ServerState.Transform);
		MoveComp->SetVelocity(ServerState.Velocity);
	
		ClearAcknowledgeMoves(ServerState.LastMove);
		for (const FP8Move& UnacknowledgeMove : UnacknowledgeMoves)
		{
			MoveComp->SimulateMove(UnacknowledgeMove);
		}
	}
	/* Simulated proxy */
	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		ClientTimeBetweenUpdates = ClientTimeSinceUpdate;
		ClientTimeSinceUpdate = 0.f;
		ClientStartTransform = GetOwner()->GetActorTransform();
		ClientStartVelocity = MoveComp->GetVelocity();
	}
}

void UP8ReplicatorComponent::ClientTick(const float DeltaTime)
{
	ClientTimeSinceUpdate += DeltaTime;
	if (ClientTimeBetweenUpdates < KINDA_SMALL_NUMBER) return;
	const float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenUpdates;
	
	/* Cubic interpolate location */
	const FVector StartLocation = ClientStartTransform.GetLocation();
	const FVector TargetLocation = ServerState.Transform.GetLocation();
	const float VelocityToDerivative = ClientTimeBetweenUpdates * 100.f;
	const FVector StartDerivative = ClientStartVelocity * VelocityToDerivative;
	const FVector TargetDerivative = ServerState.Velocity * VelocityToDerivative;
	GetOwner()->SetActorLocation(FMath::CubicInterp(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio));
	
	/* Interpolate velocity */
	const FVector NewDerivative = FMath::CubicInterpDerivative(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	MoveComp->SetVelocity(NewDerivative / VelocityToDerivative);
	
	/* Interpolate rotation */
	const FQuat TargetRotation = ServerState.Transform.GetRotation();
	GetOwner()->SetActorRotation(FQuat::Slerp(ClientStartTransform.GetRotation(), TargetRotation, LerpRatio));
}
