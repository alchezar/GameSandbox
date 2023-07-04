// Copyright (C) 2023, IKinder

#include "P7/Public/Player/P7Enemy.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P7/Public/Component/P7AttributeComponent.h"
#include "P7/Public/Item/Weapon/P7Weapon.h"
#include "P7/Public/Widget/Component/P7HealthBarComponent.h"
#include "Perception/PawnSensingComponent.h"

AP7Enemy::AP7Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	HealthBarComponent = CreateDefaultSubobject<UP7HealthBarComponent>("HealthBarWidgetComponent");
	HealthBarComponent->SetupAttachment(RootComponent);
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	PawnSensing->SetPeripheralVisionAngle(45.f);
	PawnSensing->SightRadius = 4000.f;
}

void AP7Enemy::BeginPlay()
{
	Super::BeginPlay();
	check(HealthBarComponent);
	check(PawnSensing);
	PawnSensing->OnSeePawn.AddDynamic(this, &ThisClass::OnSeePawnHandle);
	SpawnWeapon();
	HealthBarComponent->ReactOnDamage(1.f, false);
	EnemyController = Cast<AAIController>(GetController());
	ChoosePatrolTarget();
	ChangeTarget(EES_Patrolling, PatrolTarget);
}

void AP7Enemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	EnemyState > EES_Patrolling ? CheckCombatTarget() : CheckPatrolTarget();  
}

void AP7Enemy::SpawnWeapon()
{
	if (!WeaponClass) return;
	Weapon = GetWorld()->SpawnActor<AP7Weapon>(WeaponClass);
	if (!Weapon) return;
	Weapon->Equip(GetMesh(), HandSocketName, this, this);
	Weapon->AttachToHand(GetMesh(), HandSocketName);
}

void AP7Enemy::GetHit(const FVector& ImpactPoint)
{
	Super::GetHit(ImpactPoint);
}

bool AP7Enemy::GetIsAttaching()
{
	return EnemyState == EES_Attaching;
}

void AP7Enemy::Attack()
{
	Super::Attack();
	if (!CanAttack()) return;
	EnemyState = EES_Attaching;
}

void AP7Enemy::Die()
{
	Super::Die();
	HealthBarComponent->ReactOnDamage(0.f, false);
}

float AP7Enemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Attributes->ReceiveDamage(DamageAmount);
	HealthBarComponent->ReactOnDamage(Attributes->GetHealthPercent(), true);
	CombatTarget = EventInstigator->GetPawn();
	ChangeTarget(EES_Chasing, CombatTarget);
	return DamageAmount;
}

void AP7Enemy::Destroyed()
{
	Super::Destroyed();
	if (Weapon)
	{
		Weapon->Unequip();
	}
}

bool AP7Enemy::InTargetRange(const AActor* Target, const float Radius)
{
	if (!Target) return false;
	return FVector::Dist(Target->GetActorLocation(), GetActorLocation()) <= Radius;
}

void AP7Enemy::ChangeTarget(const EEnemyState NewState, AActor* NewTarget)
{
	EnemyState = NewState;
	GetCharacterMovement()->MaxWalkSpeed = EnemyState > EES_Patrolling ? 600.f : 300.f;
	MoveToTarget(NewTarget);
}

void AP7Enemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, CombatRadius))
	{
		/* Outside combat radius, loose interest */
		HealthBarComponent->SetVisibility(false);
		CombatTarget = nullptr;
		ChangeTarget(EES_Patrolling, PatrolTarget);
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EES_Chasing)
	{
		/* Inside combat radius but outside attack radius */
		ChangeTarget(EES_Chasing, CombatTarget);
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EES_Attaching)
	{
		//Play anim montage
		Attack();
	}
}

void AP7Enemy::CheckPatrolTarget()
{
	if (!InTargetRange(PatrolTarget, PatrolRadius)) return;
	ChoosePatrolTarget();
	const float Waiting = FMath::RandRange(PatrolTime.Min, PatrolTime.Max);
	GetWorld()->GetTimerManager().SetTimer(PatrolTimer, this, &ThisClass::PatrolTimerFinished, Waiting);
}

void AP7Enemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target == PatrolTarget) continue;
		ValidTargets.Add(Target);
	}
	if (ValidTargets.IsEmpty()) return;
	PatrolTarget = ValidTargets[FMath::RandRange(0, ValidTargets.Num() - 1)];
}

void AP7Enemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void AP7Enemy::MoveToTarget(AActor* Target)
{
	if (!Target || !EnemyController) return;
	EnemyController->MoveToActor(Target, 15.f);
}

void AP7Enemy::OnSeePawnHandle(APawn* Pawn)
{
	if (!Pawn->ActorHasTag("Player")) return;
	
	GetWorld()->GetTimerManager().ClearTimer(PatrolTimer);
	if (EnemyState == EES_Patrolling)
	{
		CombatTarget = Pawn;
		ChangeTarget(EES_Chasing, CombatTarget);
	}
}
