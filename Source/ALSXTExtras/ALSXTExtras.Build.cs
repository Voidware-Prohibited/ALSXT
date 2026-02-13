using UnrealBuildTool;

public class ALSXTExtras : ModuleRules
{
	public ALSXTExtras(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		CppCompileWarningSettings.NonInlinedGenCppWarningLevel = WarningLevel.Warning;

		PublicDependencyModuleNames.AddRange([
			"Core",
			"Landscape",
			"CoreUObject",
			"Engine",
			"UMG",
			"GameplayTags",
			"ModularGameplayActors"
		]);			
		
		PrivateDependencyModuleNames.AddRange([
			"PhysicsCore",
			"Slate",
			"SlateCore",
		]);

		if (Target.Type == TargetRules.TargetType.Editor)
		{
			PrivateDependencyModuleNames.AddRange([
				"MessageLog"
			]);
		}

		SetupIrisSupport(Target);
	}
}
