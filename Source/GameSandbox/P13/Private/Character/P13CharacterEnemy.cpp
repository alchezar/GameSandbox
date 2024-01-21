// Copyright © 2024, IKinder

#include "P13/Public/Character/P13CharacterEnemy.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P13/Public/Component/Actor/P13ScoreComponent.h"
#include "P13/Public/Game/P13GameMode.h"

AP13CharacterEnemy::AP13CharacterEnemy()
{
	bUseControllerRotationYaw = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	CreateEnemyComponents();
}

void AP13CharacterEnemy::BeginPlay()
{
	Super::BeginPlay();
	UpdateInventoryAfterRespawn();
}

void AP13CharacterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveToPlayer();	
}

void AP13CharacterEnemy::OnDeathHandle(AController* Causer)
{
	Super::OnDeathHandle(Causer);

	if (AP13GameMode* GameMode = GetWorld()->GetAuthGameMode<AP13GameMode>())
	{
		GameMode->RespawnEnemies();
	}
}

void AP13CharacterEnemy::CreateEnemyComponents()
{
	ScoreComp = CreateDefaultSubobject<UP13ScoreComponent>("EnemyScoreComponent");
}

void AP13CharacterEnemy::MoveToPlayer() const
{
	if (!ControllerCached.IsValid() || !ControllerCached->IsA<AAIController>())
	{
		return;
	}
	AActor* Target = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!Target)
	{
		return;
	}
	const float DistSqr = FVector::DistSquared(GetActorLocation(), Target->GetActorLocation());
	if (DistSqr < FMath::Pow(AcceptanceRadius, 2))
	{
		if (GetVelocity().Size() > 10.f)
		{
			ControllerCached->StopMovement();
		}
		return;
	}
	if (GetVelocity().Size() > 10.f)
	{
		return;
	}
	AAIController* AIController = StaticCast<AAIController*>(ControllerCached.Get());
	AIController->MoveToActor(Target);
}
