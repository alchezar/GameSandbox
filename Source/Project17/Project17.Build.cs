using UnrealBuildTool;

public class Project17 : ModuleRules
{
	public Project17(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"GameplayTags",
			"EnhancedInput",
			"GameplayAbilities",
			"GameplayTasks",
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"UMG"
		});

		PublicIncludePaths.Add("Project17");
	}
}