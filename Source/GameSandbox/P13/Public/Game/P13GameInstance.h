// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "P13/Public/Library/P13Types.h"
#include "P13GameInstance.generated.h"

UCLASS()
class GAMESANDBOX_API UP13GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FP13WeaponInfo* GetWeaponInfoByID(const FName WeaponID) const;
	FP13WeaponDrop* GetWeaponDropByID(const FName WeaponID) const;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Weapon")
	UDataTable* WeaponInfoTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Weapon")
	UDataTable* WeaponDropTable = nullptr;
};
