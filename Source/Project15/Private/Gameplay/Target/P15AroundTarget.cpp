// Copyright © 2025, Ivan Kinder

#include "Gameplay/Target/P15AroundTarget.h"

#include "Abilities/GameplayAbility.h"

AP15AroundTarget::AP15AroundTarget()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP15AroundTarget::BeginPlay()
{
	Super::BeginPlay();
}

void AP15AroundTarget::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP15AroundTarget::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	PrimaryPC = Ability->GetOwningActorFromActorInfo()->GetInstigatorController<APlayerController>();
}

void AP15AroundTarget::ConfirmTargeting()
{
	FGameplayAbilityTargetDataHandle       TargetData  = {};
	FGameplayAbilityTargetData_ActorArray* ActorsArray = new FGameplayAbilityTargetData_ActorArray{};
	ActorsArray->SetActors(GetActorsAround());
	TargetData.Add(ActorsArray);

	TargetDataReadyDelegate.Broadcast(TargetData);

	// Allow confirmation only once.
	// Or the player may unexpectedly confirm it again after using another ability with the targeted actors.
	bConfirmAllowed = false;
}

TArray<TWeakObjectPtr<AActor>> AP15AroundTarget::GetActorsAround()
{
	TArray<FOverlapResult> Overlaps;
	const APawn*           Pawn     = PrimaryPC->GetPawn();
	const FVector          Position = Pawn->GetActorLocation();
	FCollisionQueryParams  Params   = {};
	Params.AddIgnoredActor(Pawn);
	GetWorld()->OverlapMultiByObjectType(Overlaps, Position, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(Radius), Params);

	TArray<TWeakObjectPtr<AActor>> OverlappedActors = {};
	Algo::Transform(Overlaps, OverlappedActors,
		[](const FOverlapResult& Result) -> TWeakObjectPtr<AActor> { return Result.GetActor(); });

	return OverlappedActors;
}
