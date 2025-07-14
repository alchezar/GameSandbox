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
			"AIModule"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"UMG",
			"MotionWarping",
			"Niagara",
			"NavigationSystem"
		});

		PublicIncludePaths.Add("Project17");
	}
}