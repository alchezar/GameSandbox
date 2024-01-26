// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "P13/Public/Intearface/P13NetworkInterface.h"
#include "P13/Public/Library/P13Types.h"
#include "P13GameInstance.generated.h"

UCLASS()
class GAMESANDBOX_API UP13GameInstance : public UGameInstance, public IP13NetworkInterface
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	UP13GameInstance() {}
	
	/* ----------------------------- Interface ----------------------------- */
public:
	virtual void CreateSession() override;
	virtual void JoinSession() override;
	
	/* ------------------------------- This -------------------------------- */
public:
	FP13WeaponInfo* GetWeaponInfoByID(const FName WeaponID) const;
	FP13WeaponDrop* GetWeaponDropByID(const FName WeaponID) const;

	/* ----------------------------- Variables ----------------------------- */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Weapon")
	UDataTable* WeaponInfoTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Weapon")
	UDataTable* WeaponDropTable = nullptr;
};
