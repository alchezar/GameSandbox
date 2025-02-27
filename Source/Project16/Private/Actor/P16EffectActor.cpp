// Copyright © 2025, Ivan Kinder

#include "Actor/P16EffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemTestAttributeSet.h"
#include "Project16.h"
#include "AbilitySystem/P16AttributeSet.h"
#include "Components/SphereComponent.h"

AP16EffectActor::AP16EffectActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(Mesh.Get());

	Sphere = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	Sphere->SetupAttachment(GetRootComponent());
}

void AP16EffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlapCallback);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlapCallback);
}

void AP16EffectActor::OnBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: Change this to apply a Gameplay Effect. For now, using const_cast as a hack!
	const IAbilitySystemInterface* ActorWithAbilities = Cast<IAbilitySystemInterface>(OtherActor);
	EARLY_RETURN_IF(!ActorWithAbilities)
	const UAbilitySystemComponent* AbilityComponent = ActorWithAbilities->GetAbilitySystemComponent();
	EARLY_RETURN_IF(!AbilityComponent)
	const UP16AttributeSet* AttributeSet = Cast<UP16AttributeSet>(AbilityComponent->GetAttributeSet(UP16AttributeSet::StaticClass()));
	EARLY_RETURN_IF(!AttributeSet)
	UP16AttributeSet* MutableAttributeSet = const_cast<UP16AttributeSet*>(AttributeSet);
	if (bHealth)
	{
		MutableAttributeSet->SetHealth(AttributeSet->GetHealth() + 25.f);
	}
	else
	{
		MutableAttributeSet->SetMana(AttributeSet->GetMana() + 25.f);
	}
	Destroy();
}

void AP16EffectActor::OnEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{}
