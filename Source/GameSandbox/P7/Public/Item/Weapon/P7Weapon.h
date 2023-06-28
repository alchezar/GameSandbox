// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P7/Public/Item/P7Item.h"
#include "P7/Public/Player/CharacterTypes.h"
#include "P7Weapon.generated.h"

class AP7Character;
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
	void OnAttackEndHandle();
	FORCEINLINE ECharacterState GetWeaponState() const { return WeaponState; };
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; };
	void SetWeaponCollision(const ECollisionEnabled::Type CollisionType);
	void SetLastTickLocation(const FVector& LastLocation);

	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

private:
	void HitTrace();

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | State")
	TEnumAsByte<ECharacterState> WeaponState = ECharacterState::ECS_OneHanded;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UBoxComponent* WeaponBox;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USceneComponent* TraceStart;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USceneComponent* TraceEnd;

private:
	UPROPERTY()
	AP7Character* OwnerAsCharacter;
	FVector LastTickLocation = FVector::ZeroVector;
	bool bAlreadyHit = false; // Prevent multiple hits at the same attack 
};
