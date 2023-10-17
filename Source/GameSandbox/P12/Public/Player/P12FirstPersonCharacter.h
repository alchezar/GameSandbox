// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12BaseCharacter.h"
#include "P12FirstPersonCharacter.generated.h"

UCLASS()
class GAMESANDBOX_API AP12FirstPersonCharacter : public AP12BaseCharacter
{
	GENERATED_BODY()

public:
	explicit AP12FirstPersonCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void MoveInput(const FInputActionValue& Value) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	USkeletalMeshComponent* FirstPersonMesh;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditAnywhere, Category = "C++ | Component")
	FName SocketName = FName("CamSocketNeck");
};
