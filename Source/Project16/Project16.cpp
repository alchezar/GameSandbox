// Copyright © 2025, Ivan Kinder

#include "Project16.h"

IMPLEMENT_MODULE(FDefaultModuleImpl, Project16);

P16::Rich::FRichString& P16::Rich::FRichString::Add(const FString& StyleName, const FString& Text)
{
	String += StyleName + Text + End;
	return *this;
}

P16::Rich::FRichString& P16::Rich::FRichString::Num(const FString& StyleName, const int32 Number)
{
	String += StyleName + FString::FormatAsNumber(Number) + End;
	return *this;
}

P16::Rich::FRichString& P16::Rich::FRichString::Num(const FString& StyleName, const float Number)
{
	String += StyleName + FString::SanitizeFloat(Number) + End;
	return *this;
}

P16::Rich::FRichString& P16::Rich::FRichString::Gap(const FString& GapName)
{
	String += GapName;
	return *this;
}

FString P16::Rich::FRichString::Get()
{
	return String;
}
