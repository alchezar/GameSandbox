// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P7/Public/Item/P7Item.h"
#include "P7/Public/Player/CharacterTypes.h"
#include "P7Weapon.generated.h"

UCLASS()
class GAMESANDBOX_API AP7Weapon : public AP7Item
{
	GENERATED_BODY()

public:
	AP7Weapon();
	virtual void Tick(float DeltaTime) override;
	void Equip(USceneComponent* InParent, FName SocketName);
	FORCEINLINE ECharacterState GetWeaponState() const { return WeaponState; };

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | State")
	TEnumAsByte<ECharacterState> WeaponState = ECharacterState::ECS_OneHanded;
};
