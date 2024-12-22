// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameSandboxEditorTarget : TargetRules
{
	public GameSandboxEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		// IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("GameSandbox");
		ExtraModuleNames.Add("Project01");
		ExtraModuleNames.Add("Project02");
		ExtraModuleNames.Add("Project03");
		ExtraModuleNames.Add("Project04");
		ExtraModuleNames.Add("Project05");
		ExtraModuleNames.Add("Project06");
		ExtraModuleNames.Add("Project07");
		ExtraModuleNames.Add("Project08");
		ExtraModuleNames.Add("Project09");
		ExtraModuleNames.Add("Project10");
		ExtraModuleNames.Add("Project11");
		ExtraModuleNames.Add("Project12");
		ExtraModuleNames.Add("Project13");
		ExtraModuleNames.Add("Project14");
	}
}
