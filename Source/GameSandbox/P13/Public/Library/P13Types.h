// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P13Types.generated.h"

UENUM(BlueprintType)
enum class EP13MovementState : uint8
{
    Aim    UMETA(DisplayName = "Aim State"),
    Walk   UMETA(DisplayName = "Walk State"),
    Run    UMETA(DisplayName = "Run State"),
    Sprint UMETA(DisplayName = "Spring State")
};

USTRUCT(BlueprintType)
struct FP13MovementSpeed
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Aim = 200.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Walk = 300.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Run = 600.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Sprint = 900.f;
};

UCLASS()
class GAMESANDBOX_API UP13Types : public UBlueprintFunctionLibrary
{

GENERATED_BODY()

public:
    UP13Types(){};
};
