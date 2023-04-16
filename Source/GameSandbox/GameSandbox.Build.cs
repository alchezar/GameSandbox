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
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new[] { "EnhancedInput", "Slate", "SlateCore" });

		PublicIncludePaths.AddRange(new[]
		{
			"GameSandbox/P2/Animation",
			"GameSandbox/P2/Component",
			"GameSandbox/P2/Dev",
			"GameSandbox/P2/Pickup",
			"GameSandbox/P2/Player",
			"GameSandbox/P2/PlayerAI",
			"GameSandbox/P2/PlayerAI/Service",
			"GameSandbox/P2/PlayerAI/Task",
			"GameSandbox/P2/UI",
			"GameSandbox/P2/Weapon",
			"GameSandbox/P2/Weapon/Component",
			"GameSandbox/P2/Weapon/Projectile",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}