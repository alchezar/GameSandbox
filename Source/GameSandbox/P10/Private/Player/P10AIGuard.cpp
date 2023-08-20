// Copyright (C) 2023, IKinder

#include "P10/Public/Player/P10AIGuard.h"

#include "Perception/PawnSensingComponent.h"

AP10AIGuard::AP10AIGuard()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
}

void AP10AIGuard::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->OnSeePawn.AddDynamic(this, &ThisClass::OnSeePawnHandle);
	PawnSensing->OnHearNoise.AddDynamic(this, &ThisClass::OnHearNoiseHandle);
}

void AP10AIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP10AIGuard::OnSeePawnHandle(APawn* Pawn)
{
	DrawDebugString(GetWorld(), Pawn->GetActorLocation(), FString::Printf(TEXT("See")), nullptr, FColor::Green, 2.f);
}

void AP10AIGuard::OnHearNoiseHandle(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	DrawDebugString(GetWorld(), Location, FString::Printf(TEXT("Hear")), nullptr, FColor::Blue, 2.f);
}
