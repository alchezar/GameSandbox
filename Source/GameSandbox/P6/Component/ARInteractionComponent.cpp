// Copyright (C) 2023, IKinder

#include "ARInteractionComponent.h"
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

void UARInteractionComponent::PrimaryInteract()
{
	APawn* Owner = Cast<APawn>(GetOwner());
	if (!Owner) return;

	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(OUT EyeLocation, OUT EyeRotation);
	FVector SearchEndLocation = EyeLocation + EyeRotation.Vector() * 1000.f;
	
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

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

