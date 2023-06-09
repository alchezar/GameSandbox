// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameSandbox : ModuleRules
{
	public GameSandbox(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"UMG",
			"Niagara",
			"PhysicsCore",
			"AIModule",
			"GameplayTasks",
			"GameplayTags",
			"NavigationSystem",
			"ProceduralMeshComponent"
		});

		PrivateDependencyModuleNames.AddRange(new[] { "EnhancedInput", "Slate", "SlateCore", "AnimGraphRuntime" });

		PublicIncludePaths.AddRange(new[]
		{
			"GameSandbox",
			"GameSandbox/P1",
			"GameSandbox/P2",
			"GameSandbox/P3",
			"GameSandbox/P4",
			"GameSandbox/P5",
			"GameSandbox/P6"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}