// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P8/Public/Util/P8Utils.h"
#include "P8ReplicatorComponent.generated.h"

class UP8MovementComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP8ReplicatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UP8ReplicatorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void Setup(UP8MovementComponent* MoveCompRef, USceneComponent* MeshOffsetRootComp);

protected:
	virtual void BeginPlay() override;
	
	/* Multiplayer */
	UFUNCTION(Unreliable, Server, WithValidation)
	void Server_SendMove(FP8Move ServerMove);
	UFUNCTION()
	void OnRep_ServerState();

private:
	void ClearAcknowledgeMoves(FP8Move LastMove);
	void UpdateServerState(FP8Move LastMove);
	void ClientTick(const float DeltaTime);

private:
	TArray<FP8Move> UnacknowledgeMoves;
	FP8ClientData ClientData = FP8ClientData();
	UPROPERTY(ReplicatedUsing = "OnRep_ServerState")
	FP8State ServerState;
	UPROPERTY()
	UP8MovementComponent* MoveComp;
	UPROPERTY()
	USceneComponent* OffsetRt;
};
