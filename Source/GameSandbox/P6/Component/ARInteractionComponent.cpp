// Copyright (C) 2023, IKinder

#include "ARInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"
#include "P6/Interface/ARGameplayInterface.h"

UARInteractionComponent::UARInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UARInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UARInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UARInteractionComponent::PrimaryInteract(APawn* InstigatorPawn)
{
	ServerInteract(InstigatorPawn);
}

void UARInteractionComponent::ServerInteract_Implementation(APawn* InstigatorPawn)
{
	// AARCharacter* Owner = Cast<AARCharacter>(GetOwner());
	APawn* Owner = Cast<APawn>(InstigatorPawn);
	if (!Owner) return;

	FVector EyeLocation = Owner->GetPawnViewLocation();
	FRotator EyeRotation = Owner->GetViewRotation();

	// Owner->GetActorEyesViewPoint(OUT EyeLocation, OUT EyeRotation);
	FVector SearchEndLocation = EyeLocation + EyeRotation.Vector() * SearchDistance;
	
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	DrawDebugLine(GetWorld(), EyeLocation, SearchEndLocation, FColor::Green, false, 5.f);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, EyeLocation, SearchEndLocation, QueryParams);
	if (!HitResult.bBlockingHit) return;

	AActor* HitActor = HitResult.GetActor();
	if (!HitActor) return;

	if (HitActor->Implements<UARGameplayInterface>())
	{
		IARGameplayInterface::Execute_Interact(HitActor, Owner);
	}
}

