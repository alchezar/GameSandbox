// Copyright (C) 2023, IKinder

#include "AI/ARAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Component/ARAbilityComponent.h"
#include "Component/ARAttributesComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/ARDamagePopUp.h"
#include "UI/ARWorldUserWidget.h"

AARAICharacter::AARAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

#if 0
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
#endif
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AttributesComp = CreateDefaultSubobject<UARAttributesComponent>("AttributesComponent");
	AbilityComp = CreateDefaultSubobject<UARAbilityComponent>("AbilityComponent");
}

void AARAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

#if 0
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ThisClass::OnPawnSeen);
#endif

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
		// HealthBarWidget->AddToViewport();
		// HealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
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

void AARAICharacter::OnHealthChangedHandle(AActor* InstigatorActor, UARAttributesComponent* OwningComp, float NewHealth, float Delta)
{
	if (InstigatorActor == this) return;

	if (Delta < 0.f)
	{
		SetTargetActor(InstigatorActor);

		if (HealthBarWidget)
		{
			HealthBarWidget->AddToViewport();
			// HealthBarWidget->SetVisibility(ESlateVisibility::Visible);
			HealthBarWidget->SetNewHealthPercent(NewHealth / OwningComp->GetHealthMax());
		}

		if (UARDamagePopUp* DamagePopUp = CreateWidget<UARDamagePopUp>(GetWorld(), DamagePopupWidgetClass))
		{
			DamagePopUp->SetActorToAttach(this);
			DamagePopUp->AddToViewport(1);
			DamagePopUp->SetDamageText(FMath::Abs(Delta));
		}

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

void AARAICharacter::OnPawnSeen(APawn* Pawn)
{
	if (Pawn == GetTargetActor()) return;
	
	SetTargetActor(Pawn);
	MulticastPawnSeen(Pawn);
}

AActor* AARAICharacter::GetTargetActor() const
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return nullptr;
	const UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard) return nullptr;

	return Cast<AActor>(Blackboard->GetValueAsObject("TargetActor"));
}

void AARAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return;

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard) return;

	Blackboard->SetValueAsObject("TargetActor", NewTarget);
}

void AARAICharacter::MulticastPawnSeen_Implementation(APawn* Pawn)
{
	DrawDebugString(GetWorld(), Pawn->GetActorLocation(), FString::Printf(TEXT("❗")), nullptr, FColor::Red, 2.f);
}

void AARAICharacter::SetTeamColor(const FLinearColor& Color)
{
	TeamColor = Color;
	
	UMaterialInstanceDynamic* DynamicBodyMaterial1 = GetMesh()->CreateAndSetMaterialInstanceDynamic(1);
	UMaterialInstanceDynamic* DynamicBodyMaterial2 = GetMesh()->CreateAndSetMaterialInstanceDynamic(2);
	UMaterialInstanceDynamic* DynamicBodyMaterial3 = GetMesh()->CreateAndSetMaterialInstanceDynamic(3);
	if (DynamicBodyMaterial1 && DynamicBodyMaterial2 && DynamicBodyMaterial3)
	{
		DynamicBodyMaterial1->SetVectorParameterValue(TeamColorParameterName, TeamColor);
		DynamicBodyMaterial2->SetVectorParameterValue(TeamColorParameterName, TeamColor);
		DynamicBodyMaterial3->SetVectorParameterValue(TeamColorParameterName, TeamColor);
	}
}