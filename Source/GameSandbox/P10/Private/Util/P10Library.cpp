// Copyright (C) 2023, IKinder

#include "P10/Public/Util/P10Library.h"

void UP10Library::InteractWithPhysical(AActor* CubeActor, UPrimitiveComponent* CubeComp, const AActor* Projectile)
{
	if (!CubeActor || CubeActor == Projectile || !CubeComp || !CubeComp->IsSimulatingPhysics()) return;
	
	/* Add impulse to hit component. */
	const float RandomIntensity = FMath::RandRange(200.f, 500.f);
	CubeComp->AddImpulseAtLocation(Projectile->GetVelocity() * RandomIntensity, Projectile->GetActorLocation());

	/* Make hit component smaller after each bounce. */
	const FVector Scale = CubeComp->GetComponentScale() * 0.8f;
	if (Scale.GetMin() < 0.5f)
	{
		CubeActor->Destroy();
	}
	Scale.GetMin() < 0.5f ? CubeActor->Destroy() : CubeActor->SetActorScale3D(Scale);

	/* Set random color on hit component. */
	if (UMaterialInstanceDynamic* DynamicMaterialInstance = CubeComp->CreateAndSetMaterialInstanceDynamic(0))
	{
		const FLinearColor NewColor = FLinearColor::MakeRandomColor();
		DynamicMaterialInstance->SetVectorParameterValue("SurfaceColor", NewColor);
	}
}

void UP10Library::DrawDebugShoot(const UObject* WorldContextObject, const FHitResult& Hit)
{
	const UWorld* World = WorldContextObject->GetWorld();
	if (!World) return;
	
	if (Hit.bBlockingHit)
	{
		DrawDebugLine(World, Hit.TraceStart, Hit.ImpactPoint, FColor::Red, false, 10.f);
		DrawDebugLine(World, Hit.ImpactPoint, Hit.TraceEnd, FColor::Green, false, 10.f);
		DrawDebugPoint(World, Hit.ImpactPoint, 10.f, FColor::Red, false, 10.f);
	}
	else
	{
		DrawDebugLine(World, Hit.TraceStart, Hit.TraceEnd, FColor::Green, false, 10.f);
	}
}

void UP10Library::DrawDebugExplode(const UObject* WorldContextObject, const FHitResult& Hit, const float Radius)
{
	const UWorld* World = WorldContextObject->GetWorld();
	if (!World) return;

	DrawDebugCapsule(World, Hit.Location, Radius / 2, Radius, FQuat::Identity, FColor::Red, false, 10.f);
}
