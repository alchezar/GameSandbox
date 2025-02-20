// Copyright © 2025, Ivan Kinder

using UnrealBuildTool;

public class Project16 : ModuleRules
{
	public Project16(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Niagara",
			"UMG",
			"AIModule"
		});

		PublicIncludePaths.Add("Project16");
	}
}