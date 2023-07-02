// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P7/Public/Item/P7Item.h"
#include "P7/Public/Player/CharacterTypes.h"
#include "P7Weapon.generated.h"

class AP7Character;
class UBoxComponent;
class UFieldSystemComponent;
class URadialVector;
class URadialFalloff;
class UFieldSystemMetaDataFilter;

UCLASS()
class GAMESANDBOX_API AP7Weapon : public AP7Item
{
	GENERATED_BODY()

public:
	AP7Weapon();
	virtual void Tick(float DeltaTime) override;
	virtual void SwitchWeapon(const bool bOn);
	virtual void SwitchWeaponHard(const bool bOn);
	void Equip(USceneComponent* InParent, FName SocketName, const FSnapOffset& Offset, AActor* NewOwner, APawn* NewInstigator);
	void AttachToSocket(USceneComponent* InParent, FName SocketName, const FSnapOffset& Offset);
	void OnAttackStartHandle();
	void OnAttackEndHandle();
	FORCEINLINE ECharacterState GetWeaponState() const { return WeaponState; };
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; };
	void SetWeaponCollision(const ECollisionEnabled::Type CollisionType);
	void SetLastTickLocation(const FVector& LastLocation);

	UFUNCTION()
	virtual void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	virtual void BeginPlay() override;
	virtual void SwitchRibbon(const bool bOn);
	virtual void SplashEffect(const FHitResult& HitResult);
	void CreateFields(const FVector& FieldLocation);

private:
	void HitTrace();

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | State")
	TEnumAsByte<ECharacterState> WeaponState = ECharacterState::ECS_OneHanded;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UBoxComponent* WeaponBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	FP7NiagaraEffect Ribbon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	FP7NiagaraEffect Contact;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Effect")
	FP7WeaponSound WeaponSound;

	UPROPERTY(EditAnywhere, Category = "C++ | Damage")
	float Damage = 20.f;
	UPROPERTY(EditAnywhere, Category = "C++ | Damage")
	float FieldMagnitude = 500000.f;
	
private:
	UPROPERTY()
	AP7Character* OwnerAsCharacter;
	FVector LastTickLocation = FVector::ZeroVector;
	bool bAlreadyHit = false; // Prevent multiple hits at the same attack

	UPROPERTY()
	UFieldSystemComponent* FieldSystem;
	UPROPERTY()
	URadialVector* RadialVector;
	UPROPERTY()
	URadialFalloff* RadialFalloff;
	UPROPERTY()
	UFieldSystemMetaDataFilter* MetaDataFilter;
};

