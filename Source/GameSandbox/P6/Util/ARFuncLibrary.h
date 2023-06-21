// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ARFuncLibrary.generated.h"

UCLASS()
class GAMESANDBOX_API UARFuncLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "C++ | Utils")
	static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount);
	UFUNCTION(BlueprintCallable, Category = "C++ | Utils")
	static bool ApplyDirectDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult, float Impulse = 2000.f);
	UFUNCTION(BlueprintCallable, Category = "C++ | Utils")
	static void LogOnScreen (UObject* WorldContext, const FString& Msg, FColor Color = FColor::White, float Duration = 5.f);
};
