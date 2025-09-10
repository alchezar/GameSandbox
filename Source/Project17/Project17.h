#pragma once

#include "CoreMinimal.h"

///
/// @brief Custom macros magic.
///

DEFINE_LOG_CATEGORY_STATIC(LogP17, All, All)

///
/// @brief Early returning macro, without messages.
///
#define RETURN_IF(Reason, Value) \
if (Reason) _UNLIKELY            \
{                                \
	return Value;                \
}

///
/// @brief Continue macro in one line.
///
#define CONTINUE_IF(Reason) \
if (Reason)                 \
{                           \
	continue;               \
}

///
/// @brief Early returning macro, with an ability to show log/screen warn messages.
/// @attention Works only in Debug and Development builds.
///
#define WARN_RETURN_IF(Reason, Value) \
if (Reason) _UNLIKELY                 \
{                                     \
	LOUD_WARN(Reason)                 \
	return Value;                     \
}

///
/// @brief Continue macro in one line.
///
#define WARN_CONTINUE_IF(Reason) \
if (Reason)                      \
{                                \
	LOUD_WARN(Reason)            \
	continue;                    \
}

///
/// @brief Warn if the Reason is bad but don't crash the application.
/// @attention Won't work in shipping and test builds.
///
#define WARN_UNLESS(Reason) \
if (!Reason) _UNLIKELY      \
{                           \
	LOUD_WARN(!Reason)      \
}                           \
else

///
/// @brief Show a warn message in the log and on the screen.
/// @attention Works only in Debug and Development builds.
///
#define LOUD_WARN(Reason)                          \
if (UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT)        \
{                                                  \
	GEngine->AddOnScreenDebugMessage(              \
		-1, 10.f, FColor::Red,                     \
		FString::Printf(WARN_MESSAGE(Reason)));    \
	UE_LOG(LogP17, Warning, WARN_MESSAGE(Reason)); \
}

///
/// @brief Warning message, with the reason, function and line number.
///
#define WARN_MESSAGE(Reason) \
L"[%hs] in [%hs] at [%d]",   \
#Reason,                     \
__FUNCTION__,                \
__LINE__
