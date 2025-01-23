// Copyright © 2024, Ivan Kinder

using UnrealBuildTool;

public class Project11 : ModuleRules
{
	public Project11(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"OnlineSubsystem"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"UMG",
			"SlateCore",
			"EnhancedInput"
		});
	}
}