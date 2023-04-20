// Copyright (C) 2023, IKinder

#include "STU_PlayerStart.h"

// Sets default values
ASTU_PlayerStart::ASTU_PlayerStart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASTU_PlayerStart::BeginPlay()
{
	Super::BeginPlay();	
}