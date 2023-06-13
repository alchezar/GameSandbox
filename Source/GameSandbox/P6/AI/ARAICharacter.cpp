// Copyright (C) 2023, IKinder

#include "ARAICharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P6/Component/ARAbilityComponent.h"
#include "P6/Component/ARAttributesComponent.h"
#include "P6/UI/ARWorldUserWidget.h"
#include "Perception/PawnSensingComponent.h"

AARAICharacter::AARAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AttributesComp = CreateDefaultSubobject<UARAttributesComponent>("AttributesComponent");
	AbilityComp = CreateDefaultSubobject<UARAbilityComponent>("AbilityComponent");
}

void AARAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ThisClass::OnPawnSeen);

	if (AttributesComp)
	{
		AttributesComp->AROnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChangedHandle);
	}
}

void AARAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidgetClass)
	{
		HealthBarWidget = CreateWidget<UARWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
		HealthBarWidget->AttachedActor = this;
		HealthBarWidget->AddToViewport();
		HealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AARAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AARAICharacter::IsAlive() const
{
	return true;
}

void AARAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), Pawn->GetActorLocation(), FString::Printf(TEXT("See you")), nullptr, FColor::Green, 2.f);
}

void AARAICharacter::OnHealthChangedHandle(AActor* InstigatorActor, UARAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	if (InstigatorActor == this) return;

	if (Delta < 0.f)
	{
		SetTargetActor(InstigatorActor);

		HealthBarWidget->SetVisibility(ESlateVisibility::Visible);
		HealthBarWidget->SetNewHealthPercent(NewHealth / OwningComp->GetHealthMax());

		if (FMath::IsNearlyZero(NewHealth))
		{
			// Stop BP
			const AAIController* AIController = Cast<AAIController>(GetController());
			if (!AIController) return;

			AIController->GetBrainComponent()->StopLogic("Killed");

			// Ragdoll
			GetCharacterMovement()->DisableMovement();
			GetCharacterMovement()->MovementState.bCanJump = false;
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			SetLifeSpan(10.f);
		}
	}
}

void AARAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return;

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard) return;

	Blackboard->SetValueAsObject("TargetActor", NewTarget);
}
