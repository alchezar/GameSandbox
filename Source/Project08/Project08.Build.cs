// Copyright © 2024, Ivan Kinder

using UnrealBuildTool;

public class Project08 : ModuleRules
{
	public Project08(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"OnlineSubsystem",
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"OnlineSubsystemUtils",
			"UMG"
		});
	}
}
