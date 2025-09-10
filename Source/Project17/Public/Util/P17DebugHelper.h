// Copyright © 2025, Ivan Kinder

#pragma once
#include "Project17.h"

namespace P17::Debug
{
	static void Print(const FString& Message, const int32 Key = -1, const FColor& Color = FColor::MakeRandomColor())
	{
		RETURN_IF(!GEngine,)
		GEngine->AddOnScreenDebugMessage(Key, 5.f, Color, Message);
		UE_LOG(LogP17, Display, L"%s", *Message)
	}
}
