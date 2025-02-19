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
			"AIModule",
			"FieldSystemEngine",
			"GeometryCollectionEngine",
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"EnhancedInput",
			"Slate",
			"SlateCore",
		});

		PublicIncludePaths.AddRange(new[]
		{
			"GameSandbox"
		});

		// DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}