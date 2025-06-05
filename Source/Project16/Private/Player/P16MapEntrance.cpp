// Copyright © 2025, Ivan Kinder

#include "Player/P16MapEntrance.h"

#include "Project16.h"
#include "Game/P16GameMode.h"
#include "Interface/P16PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Project15/Project15.h"

AP16MapEntrance::AP16MapEntrance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Mesh->SetCustomDepthStencilValue(P16::CustomDepthYellow);
}

void AP16MapEntrance::BeginPlay()
{
	Super::BeginPlay();
}

void AP16MapEntrance::LoadActor_Implementation()
{
	// Don't handle glow effect here.
}

void AP16MapEntrance::OnSphereBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EARLY_RETURN_IF(!OtherActor->Implements<UP16PlayerInterface>())

	if (const AP16GameMode* GameMode = GetWorld()->GetAuthGameMode<AP16GameMode>())
	{
		GameMode->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
	}

	IP16PlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
}
