// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameSandboxTarget : TargetRules
{
	public GameSandboxTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		ExtraModuleNames.Add("GameSandbox");
		ExtraModuleNames.Add("Project01");
	}
}
