// Copyright © 2024, IKinder

#include "Component/Actor/P13ScoreComponent.h"

#include "Component/Actor/P13HealthComponent.h"
#include "Game/P13PlayerState.h"

UP13ScoreComponent::UP13ScoreComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ConnectToHealth();
}

void UP13ScoreComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP13ScoreComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UP13ScoreComponent::OnOwnerDeadHandle(AController* Causer)
{
	if (AP13PlayerState* PlayerState = Causer ? Causer->GetPlayerState<AP13PlayerState>() : nullptr)
	{
		PlayerState->AddScore(Score);
	}
}

void UP13ScoreComponent::ConnectToHealth()
{
	UP13HealthComponent* HealthComponent = GetOwner() ? GetOwner()->FindComponentByClass<UP13HealthComponent>() : nullptr;
	if (!HealthComponent)
	{
		return;
	}
	HealthComponent->OnHealthOver.AddUObject(this, &ThisClass::OnOwnerDeadHandle);
}
