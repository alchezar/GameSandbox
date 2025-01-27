// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GameSandboxEditorTarget : TargetRules
{
	public GameSandboxEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		ExtraModuleNames.AddRange(new[]
		{
			"GameSandbox",
			"Project01",
			"Project02",
			"Project03",
			"Project04",
			"Project05",
			"Project06",
			"Project07",
			"Project08",
			"Project09",
			"Project10",
			"Project11",
			"Project12",
			"Project13",
			"Project14",
			"Project15"
		});
	}
}
