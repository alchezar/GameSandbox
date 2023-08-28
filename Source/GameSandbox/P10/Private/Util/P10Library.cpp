// Copyright (C) 2023, IKinder

#include "P10/Public/Util/P10Library.h"

#include "P10/Public/Weapon/P10Weapon.h"

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

/** Draw debugs */

/* Create ConsoleVariable with a default value of 0. */
static TAutoConsoleVariable<int32> CVarDrawDebug(TEXT("P10.DrawDebug"), 0, TEXT("Allow to draw debug helpers."), ECVF_Cheat);

/* Find the current value of the ConsoleVariable as a bool. */
bool UP10Library::GetIsDrawDebugAllowed()
{
	return IConsoleManager::Get().FindConsoleVariable(TEXT("P10.DrawDebug"))->GetBool();
}

void UP10Library::DrawDebugShoot(const UObject* WorldContextObject, const FHitResult& Hit)
{
	if (!GetIsDrawDebugAllowed()) return;
	
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
	if (!GetIsDrawDebugAllowed()) return;
	
	const UWorld* World = WorldContextObject->GetWorld();
	if (!World) return;

	DrawDebugCapsule(World, Hit.Location, Radius / 2, Radius, FQuat::Identity, FColor::Red, false, 10.f);
}

void UP10Library::DrawTargetInfo(const UObject* WorldContextObject, const FVector& Location, const FString& Text)
{
	if (!GetIsDrawDebugAllowed()) return;
	
	const UWorld* World = WorldContextObject->GetWorld();
	if (!World) return;

	DrawDebugString(World, Location, FString::Printf(TEXT("%s"), *Text), nullptr, FColor::Green, 10.f);

}

void UP10Library::DrawAmmoInfo(const UObject* WorldContextObject, const AP10Weapon* Weapon)
{
	const UWorld* World = WorldContextObject->GetWorld();
	if (!World || !GEngine || !Weapon) return;

	FString AmmoString = (Weapon->GetCurrentAmmo() / 10 == 0) ? "0" : "";
	AmmoString += FString::FromInt(Weapon->GetCurrentAmmo());

	int32 Clips = Weapon->GetAmmoInClips() / Weapon->GetClipCapacity();
	Clips += (Weapon->GetAmmoInClips() % Weapon->GetClipCapacity() == 0) ? 0 : 1;

	FString ClipsString = (Clips / 10 == 0) ? "0" : "";
	ClipsString += FString::FromInt(Clips);

	FString AmmoInClipsString = (Weapon->GetAmmoInClips() / 10 == 0) ? "0" : "";
	AmmoInClipsString += FString::FromInt(Weapon->GetAmmoInClips());
	
	FString AmmoInfo = "Ammo: ";
	AmmoInfo += AmmoString + " / " + ClipsString + " ( " + AmmoInClipsString + " )";
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, AmmoInfo);
}
