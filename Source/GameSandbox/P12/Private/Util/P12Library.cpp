// Copyright (C) 2023, IKinder

#include "P12/Public/Util/P12Library.h"

static TAutoConsoleVariable<int> CVarDrawDebug(TEXT("P12.Debug"), 0, TEXT("Allow to draw debug helpers and console logs"), ECVF_Cheat);

bool UP12Library::GetDrawDebugAllowed()
{
	return IConsoleManager::Get().FindConsoleVariable(TEXT("P12.Debug"))->GetBool();
}
