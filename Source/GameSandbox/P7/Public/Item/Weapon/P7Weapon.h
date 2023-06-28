// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P7/Public/Item/P7Item.h"
#include "P7/Public/Player/CharacterTypes.h"
#include "P7Weapon.generated.h"

class UBoxComponent;

UCLASS()
class GAMESANDBOX_API AP7Weapon : public AP7Item
{
	GENERATED_BODY()

public:
	AP7Weapon();
	virtual void Tick(float DeltaTime) override;
	void Equip(USceneComponent* InParent, FName SocketName, const FSnapOffset& Offset);
	void AttachToSocket(USceneComponent* InParent, FName SocketName, const FSnapOffset& Offset);
	FORCEINLINE ECharacterState GetWeaponState() const { return WeaponState; };
	void SetWeaponCollision(const ECollisionEnabled::Type CollisionType);

	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | State")
	TEnumAsByte<ECharacterState> WeaponState = ECharacterState::ECS_OneHanded;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UBoxComponent* WeaponBox;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USceneComponent* TraceStart;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USceneComponent* TraceEnd;
};
