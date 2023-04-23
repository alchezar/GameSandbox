// Copyright (C) 2023, IKinder

#include "STU_AICharacter.h"

#include "BrainComponent.h"
#include "STU_AIController.h"
#include "Component/STU_AIWeaponComponent.h"
#include "Component/STU_HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/STU_HealthBarWidget.h"

ASTU_AICharacter::ASTU_AICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USTU_AIWeaponComponent>("WeaponComponent"))
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI     = EAutoPossessAI::Disabled;
	AIControllerClass = ASTU_AIController::StaticClass();

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate                  = FRotator(0.0, 200.0, 0.0);
	}
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void ASTU_AICharacter::BeginPlay()
{
	Super::BeginPlay();
	check(HealthWidgetComponent);

	GetWorld()->GetTimerManager().SetTimer(HealthWidgetTimer, this, &ThisClass::UpdateHealthWidgetVisibility, 1.f, true);
}

void ASTU_AICharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTU_AICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASTU_AICharacter::OnHealthChangedHandle(float Health, float HealthDelta)
{
	Super::OnHealthChangedHandle(Health, HealthDelta);

	USTU_HealthBarWidget* HealthBarWidget = Cast<USTU_HealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
	if (!HealthBarWidget) return;

	HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
}

void ASTU_AICharacter::OnDeathHandle()
{
	Super::OnDeathHandle();

	const AAIController* AIController = Cast<AAIController>(Controller);
	if (AIController && AIController->BrainComponent)
	{
		AIController->BrainComponent->Cleanup();
	}
}

void ASTU_AICharacter::UpdateHealthWidgetVisibility()
{
	if (!GetWorld()) return;
	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;
	const APawn* Pawn = PlayerController->GetPawnOrSpectator();
	if (!Pawn) return;

	const FVector PlayerLocation = Pawn->GetActorLocation();
	const double Distance        = FVector::Dist(PlayerLocation, GetActorLocation());
	// HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
	if (USTU_HealthBarWidget* HealthBarWidget = Cast<USTU_HealthBarWidget>(HealthWidgetComponent->GetWidget()))
	{
		HealthBarWidget->SetRenderScale(FVector2D(HealthVisibilityDistance > Distance ? 1.f : HealthVisibilityDistance / Distance));
	}
}
