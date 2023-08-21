// Copyright (C) 2023, IKinder

#include "P10/Public/Game/P10GameMode.h"

#include "EngineUtils.h"
#include "P10/Public/Player/P10Character.h"
#include "P10/Public/UI/P10HUD.h"

AP10GameMode::AP10GameMode()
{
	DefaultPawnClass = AP10Character::StaticClass();
	HUDClass = AP10HUD::StaticClass();
}

void AP10GameMode::BeginPlay()
{
	Super::BeginPlay();
	check(ViewPointClass)
}

void AP10GameMode::CompleteMission(APawn* InstigatorPawn, bool bSuccess)
{
	if (!InstigatorPawn) return;

	InstigatorPawn->DisableInput(nullptr);

	APlayerController* PlayerController = Cast<APlayerController>(InstigatorPawn->Controller);
	if (!PlayerController) return;
	AActor* TargetActor = *TActorIterator<AActor>(GetWorld(), ViewPointClass);
	if (!TargetActor) return;
	
	PlayerController->SetViewTargetWithBlend(TargetActor, 0.5f, VTBlend_Cubic);

	const FString PrintMsg = bSuccess ? "MISSION COMPLETE" : "MISSION FAILED";
	const FColor MsgColor = bSuccess ? FColor::Green : FColor::Red;
	if (GEngine) GEngine->AddOnScreenDebugMessage(1, 10.f, MsgColor, PrintMsg);
}
