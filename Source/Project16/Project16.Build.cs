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
			"GameplayAbilities"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Niagara",
			"UMG",
			"AIModule",
			"GameplayTags",
			"GameplayTasks"
		});

		PublicIncludePaths.Add("Project16");
	}
}