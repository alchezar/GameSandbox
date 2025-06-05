// Copyright (C) 2023, IKinder

#include "P7/Public/Player/P7Enemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P7/Public/Component/P7AttributeComponent.h"
#include "P7/Public/Item/Weapon/P7Weapon.h"
#include "P7/Public/Widget/Component/P7HealthBarComponent.h"
#include "P7/Public/Item/P7Soul.h"

AP7Enemy::AP7Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	HealthBarComponent = CreateDefaultSubobject<UP7HealthBarComponent>("HealthBarWidgetComponent");
	HealthBarComponent->SetupAttachment(RootComponent);
#if 0
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	PawnSensing->SetPeripheralVisionAngle(45.f);
	PawnSensing->SightRadius = 4000.f;
#endif
}

void AP7Enemy::BeginPlay()
{
	Super::BeginPlay();
	check(HealthBarComponent);
	check(SoulClass);
#if 0
	check(PawnSensing);
	PawnSensing->OnSeePawn.AddDynamic(this, &ThisClass::OnSeePawnHandle);
#endif
	InitializeEnemy();
}

void AP7Enemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EnemyState == EES_Dead) return;
	EnemyState > EES_Patrolling ? CheckCombatTarget() : CheckPatrolTarget();  
}

float AP7Enemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HealthBarComponent->ReactOnDamage(Attributes->GetHealthPercent(), true);
	CombatTarget = EventInstigator->GetPawn();
	NewTargetBehavior(EES_Chasing, CombatTarget);
	return DamageAmount;
}

void AP7Enemy::Destroyed()
{
	Super::Destroyed();
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}

void AP7Enemy::GetHit(const FVector& HitterLocation)
{
	Super::GetHit(HitterLocation);
	EnemyState = EES_Patrolling;
}

bool AP7Enemy::GetIsAttaching()
{
	return EnemyState >= EES_Attaching;
}

void AP7Enemy::Die()
{
	Super::Die();
	HealthBarComponent->ReactOnDamage(0.f, false);
	EnemyState = EES_Dead;
	GetWorld()->SpawnActor<AP7Soul>(SoulClass, GetActorLocation(), FRotator::ZeroRotator);
}

void AP7Enemy::Attack()
{
	Super::Attack();
	if (!CanAttack()) return;
	EnemyState = EES_Engaged;
}

bool AP7Enemy::CanAttack()
{
	const AP7BaseCharacter* TargetCharacter = Cast<AP7BaseCharacter>(CombatTarget);
	if (!TargetCharacter) return false;
	const bool bTargetAlive = TargetCharacter->GetAttributes()->GetIsAlive();
	const bool bSelfAlive = GetAttributes()->GetIsAlive();
	
	return EnemyState != EES_Dead	   &&
		   EnemyState <= EES_Chasing   &&
		   InsideTargetRadius(CombatTarget, AttackRadius) &&
	   	   bTargetAlive &&
		   bSelfAlive;
}

void AP7Enemy::OnAttackEndHandle(USkeletalMeshComponent* MeshComp)
{
	Super::OnAttackEndHandle(MeshComp);
	if (EnemyState > EES_Chasing) EnemyState = EES_Chasing;
}

void AP7Enemy::OnHitReactEndHandle(USkeletalMeshComponent* MeshComp)
{
	Super::OnHitReactEndHandle(MeshComp);
	EnemyState = EES_Chasing;
}

void AP7Enemy::OnSeePawnHandle(APawn* Pawn)
{
	if (!Pawn->ActorHasTag("Player") || EnemyState != EES_Patrolling) return;
	
	GetWorld()->GetTimerManager().ClearTimer(PatrolTimer);
	CombatTarget = Pawn;
	NewTargetBehavior(EES_Chasing, CombatTarget);
}

void AP7Enemy::InitializeEnemy()
{
	HealthBarComponent->ReactOnDamage(1.f, false);
	EnemyController = Cast<AAIController>(GetController());
	SpawnWeapon();
	ChoosePatrolTarget();
	NewTargetBehavior(EES_Patrolling, PatrolTarget);
	Tags.Add("Enemy");
}

void AP7Enemy::SpawnWeapon()
{
	if (!WeaponClass) return;
	EquippedWeapon = GetWorld()->SpawnActor<AP7Weapon>(WeaponClass);
	check(EquippedWeapon);
	
	EquippedWeapon->Equip(GetMesh(), HandSocketName, this, this);
	EquippedWeapon->AttachToHand(GetMesh(), HandSocketName);
}

bool AP7Enemy::InsideTargetRadius(const AActor* Target, const float Radius) const
{
	return Target && Radius > FVector::Dist(Target->GetActorLocation(), GetActorLocation());
}

void AP7Enemy::MoveToTarget(AActor* Target)
{
	if (!Target || !EnemyController) return;
	EnemyController->MoveToActor(Target, 60.f);
}

void AP7Enemy::NewTargetBehavior(const EEnemyState NewState, AActor* NewTarget)
{
	EnemyState = NewState;
	GetCharacterMovement()->MaxWalkSpeed = EnemyState > EES_Patrolling ? ChasingSpeed : PatrolSpeed;
	MoveToTarget(NewTarget);
}

void AP7Enemy::CheckCombatTarget()
{
	if (!InsideTargetRadius(CombatTarget, CombatRadius))
	{
		LoseInterest();
		NewTargetBehavior(EES_Patrolling, PatrolTarget);
	}
	else if (!InsideTargetRadius(CombatTarget, AttackRadius) && EnemyState != EES_Chasing)
	{
		/* Inside combat radius but outside attack radius */
		NewTargetBehavior(EES_Chasing, CombatTarget);
	}
	else if (CanAttack())
	{
		Attack();
	}
}

void AP7Enemy::CheckPatrolTarget()
{
	if (!InsideTargetRadius(PatrolTarget, PatrolAcceptRadius)) return;
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

void AP7Enemy::LoseInterest()
{
	HealthBarComponent->SetVisibility(false);
	CombatTarget = nullptr;
}