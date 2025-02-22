// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P7/Public/Item/P7Item.h"
#include "P7/Public/Player/CharacterTypes.h"
#include "P7Weapon.generated.h"

class AP7BaseCharacter;
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
	virtual void OnAttackStartHandle();
	virtual void OnAttackEndHandle();
	virtual void Equip(USceneComponent* InParent, FName SocketName, AActor* NewOwner, APawn* NewInstigator);
	void Unequip();
	void AttachToHand(USceneComponent* InParent, const FName SocketName);
	void AttachToBelt(USceneComponent* InParent, const FName SocketName);
	FORCEINLINE ECharacterState GetWeaponState() const { return WeaponState; };
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; };
	FORCEINLINE FSnapOffset GetHandSnapOffset() const { return HandSnapOffset; };
	void SetWeaponCollision(const ECollisionEnabled::Type CollisionType);
	void SetLastTickLocation(const FVector& LastLocation);

	UFUNCTION()
	virtual void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	virtual void BeginPlay() override;
	virtual void SplashEffect(const FHitResult& HitResult);
	void CreateFields(const FVector& FieldLocation);

private:
	void HitTrace();
	void SweepCapsule(FHitResult& HitResult);
	void AttachToSocket(USceneComponent* InParent, const FName SocketName, const FSnapOffset& Offset) const;
	void EffectActivation(const bool bActive) const;

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

	UPROPERTY(EditAnywhere, Category = "C++ | Snapping")
	FSnapOffset HandSnapOffset = {FVector(0.f, 2.f, 0.f), FRotator(25.f, 0.f, 10.f)};
	UPROPERTY(EditAnywhere, Category = "C++ | Snapping")
	FSnapOffset BeltSnapOffset;
	
private:
	UPROPERTY()
	AP7BaseCharacter* OwnerChar;
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

