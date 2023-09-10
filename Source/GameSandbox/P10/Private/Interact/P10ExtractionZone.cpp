// Copyright (C) 2023, IKinder

#include "P10/Public/Interact/P10ExtractionZone.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "P10/Public/Game/P10GameMode.h"
#include "P10/Public/Player/P10Character.h"

AP10ExtractionZone::AP10ExtractionZone()
{
	OverlapComponent = CreateDefaultSubobject<UBoxComponent>("OverlapBoxComponent");
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComponent->SetBoxExtent(FVector(200.f));
	OverlapComponent->SetHiddenInGame(false);
	SetRootComponent(OverlapComponent);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>("DecalComponent");
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->SetWorldRotation(FRotator(90.f, 0.f, 0.f));
	DecalComponent->DecalSize = FVector(200.f);
}

void AP10ExtractionZone::BeginPlay()
{
	Super::BeginPlay();
	check(OverlapComponent)
	check(DecalComponent)
	check(MissingSound)
	check(CompleteSound)
	
	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxBeginOverlapHandle);
	
}

void AP10ExtractionZone::OnBoxBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AP10Character* Character = Cast<AP10Character>(OtherActor);
	if (!Character) return;

	USoundBase* CurrentSound = Character->GetIsCarryingObjective() ? CompleteSound : MissingSound;
	UGameplayStatics::SpawnSound2D(this, CurrentSound);
	
	AP10GameMode* GameMode = Cast<AP10GameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode && Character->GetIsCarryingObjective())
	{
		GameMode->CompleteMission(Character, true);
	}
}
