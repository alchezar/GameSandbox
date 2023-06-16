// Copyright (C) 2023, IKinder

#include "ARFuncLibrary.h"
#include "P6/Component/ARAttributesComponent.h"

bool UARFuncLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	if (UARAttributesComponent* Attributes = UARAttributesComponent::GetAttributes(TargetActor))
	{
		return Attributes->TryChangeHealth(DamageCauser, -DamageAmount);
	}
	return false;
}

bool UARFuncLibrary::ApplyDirectDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult, float Impulse)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (!HitComp || !HitComp->IsSimulatingPhysics(HitResult.BoneName)) return false;

		const FVector ImpactDirection = (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal();
		HitComp->AddImpulseAtLocation(ImpactDirection * Impulse, HitResult.ImpactPoint, HitResult.BoneName);
		return true;
	}
	return false;
}

void UARFuncLibrary::LogOnScreen(UObject* WorldContext, const FString& Msg, FColor Color, float Duration)
{
	if (!WorldContext || !GEngine) return;

	const UWorld* World = WorldContext->GetWorld();
	if (!World) return;

	FString NetPrefix = World->IsNetMode(NM_Client) ? "[Client] " : "[Server] ";
	GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
}
