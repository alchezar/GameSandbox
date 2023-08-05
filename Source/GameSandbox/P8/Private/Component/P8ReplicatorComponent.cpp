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
	check(OffsetRt)
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

void UP8ReplicatorComponent::Setup(UP8MovementComponent* MoveCompRef, USceneComponent* MeshOffsetRootComp)
{
	MoveComp = MoveCompRef;
	OffsetRt = MeshOffsetRootComp;
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
		ClientData.TimeBetweenUpdates = ClientData.TimeSinceUpdate;
		ClientData.TimeSinceUpdate = 0.f;
		ClientData.StartTransform = OffsetRt->GetComponentTransform();
		ClientData.StartVelocity = MoveComp->GetVelocity();

		GetOwner()->SetActorTransform(ServerState.Transform);
	}
}

void UP8ReplicatorComponent::ClientTick(const float DeltaTime)
{
	ClientData.TimeSinceUpdate += DeltaTime;
	if (ClientData.TimeBetweenUpdates < KINDA_SMALL_NUMBER) return;
	
	/* Calculate values for interpolation */
	const FP8HermiteCubicSpline Spline(ClientData, ServerState);
	
	/* Cubic interpolate location, velocity and rotation*/
	OffsetRt->SetWorldLocation(Spline.InterpolateLocation());
	MoveComp->SetVelocity(Spline.InterpolateVelocity());
	OffsetRt->SetWorldRotation(Spline.InterpolateRotation());
}
