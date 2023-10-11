// Copyright (C) 2023, IKinder

#include "P12/Public/Util/P12Library.h"

#include "HAL/IConsoleManager.h"
#include "P12/Public/Component/Actor/P12LedgeDetectionComponent.h"

static TAutoConsoleVariable CVarDrawDebug(TEXT("P12.Debug"), 0, TEXT("Allow to draw debug helpers and console logs."), ECVF_Cheat);
static TAutoConsoleVariable CVarDrawDebugPrintScreen(TEXT("P12.Debug.PrintScreen"), 0, TEXT("Allow to print screen messages."), ECVF_Cheat);
static TAutoConsoleVariable CVarDrawDebugLegAlignment(TEXT("P12.Debug.LegAlignment"), 0, TEXT("Allow to draw leg alignment debug helpers."), ECVF_Cheat);
static TAutoConsoleVariable CVarDrawDebugLedgeDetection(TEXT("P12.Debug.LedgeDetection"), 0, TEXT("Allow to draw ledge detection debug helpers."), ECVF_Cheat);

TArray<FString> UP12Library::GetAllConsoleVariables()
{	
	TArray<FString> ConsoleVariables;
	ConsoleVariables.Add("P12.Debug");
	ConsoleVariables.Add("P12.Debug.PrintScreen");
	ConsoleVariables.Add("P12.Debug.LegAlignment");
	ConsoleVariables.Add("P12.Debug.LedgeDetection");
	return ConsoleVariables;
}

bool UP12Library::GetCanDrawDebug()
{
	return IConsoleManager::Get().FindConsoleVariable(TEXT("P12.Debug"))->GetBool();
}

bool UP12Library::GetCanDrawDebugPrintScreen()
{
	return GetCanDrawDebug() && IConsoleManager::Get().FindConsoleVariable(TEXT("P12.Debug.PrintScreen"))->GetBool();
}

bool UP12Library::GetCanDrawDebugLegAlignment()
{
	return GetCanDrawDebug() && IConsoleManager::Get().FindConsoleVariable(TEXT("P12.Debug.LegAlignment"))->GetBool();
}

bool UP12Library::GetCanDrawDebugLedgeDetection()
{
	return GetCanDrawDebug() && IConsoleManager::Get().FindConsoleVariable(TEXT("P12.Debug.LedgeDetection"))->GetBool();
}

void UP12Library::DrawDebugLineTrace(const UWorld* World, const FHitResult& HitResult, const bool bDraw)
{
	if (!bDraw || !World)
	{
		return;
	}
	if (!HitResult.bBlockingHit)
	{
		DrawDebugLine(World, HitResult.TraceStart, HitResult.TraceEnd, FColor::Green);
		return;
	}
	DrawDebugLine(World, HitResult.TraceStart, HitResult.ImpactPoint, FColor::Red);
	DrawDebugLine(World, HitResult.ImpactPoint, HitResult.TraceEnd, FColor::Green);
	DrawDebugPoint(World, HitResult.ImpactPoint, 10.f, FColor::Red);
}

void UP12Library::DrawDebugCapsuleTrace(const UWorld* World, const FHitResult& HitResult, const float Radius, const float HalfHeight, const FColor Color, const bool bDraw)
{
	if (!bDraw || !World)
	{
		return;
	}

	constexpr float DrawTime = 5.f;
	constexpr float SuccessThickness = 1.f;
	
	if (HitResult.bBlockingHit)
	{
		DrawDebugCapsule(World, HitResult.Location, HalfHeight, Radius, FQuat::Identity, Color, false, DrawTime, 0, SuccessThickness);
		DrawDebugDirectionalArrow(World, HitResult.TraceStart, HitResult.Location, Radius, Color, false, DrawTime, 0, SuccessThickness);
	}
	else
	{
		DrawDebugCapsule(World, HitResult.TraceEnd, HalfHeight, Radius, FQuat::Identity, Color, false, DrawTime);
		DrawDebugDirectionalArrow(World, HitResult.TraceStart, HitResult.TraceEnd, Radius, Color, false, DrawTime, 0, SuccessThickness);
	}
	DrawDebugCapsule(World, HitResult.TraceStart, HalfHeight, Radius, FQuat::Identity, Color, false, DrawTime);
}

void UP12Library::DrawDebugDirectionalCapsule(const UWorld* World, FP12LedgeDescription& LedgeDescription, const float Radius, const float HalfHeight, const FVector& Location, const bool bDraw)
{
	if (!bDraw || !World)
	{
		return;
	}
	DrawDebugCapsule(World, Location, HalfHeight, Radius, FQuat::Identity, FColor::Red, false, 5.f, 0, 1.f);
	DrawDebugDirectionalArrow(World, Location, Location + LedgeDescription.Rotation.Vector() * 50.f, 50.f, FColor::Red, false, 5.f, 0, 1.f);
}
