// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameSandboxEditorTarget : TargetRules
{
	public GameSandboxEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		ExtraModuleNames.Add("GameSandbox");
		ExtraModuleNames.Add("Project01");
		ExtraModuleNames.Add("Project14");
	}
}
