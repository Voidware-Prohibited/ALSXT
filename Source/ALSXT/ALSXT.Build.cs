using UnrealBuildTool;

public class ALSXT : ModuleRules
{
	public ALSXT(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		// CppCompileWarningSettings.UnsafeTypeCastWarningLevel = WarningLevel.Warning;
		CppCompileWarningSettings.NonInlinedGenCppWarningLevel = WarningLevel.Warning;

		PublicDependencyModuleNames.AddRange([
			"Core",
			"Landscape",
			"CoreUObject",
			"Engine",
			"AudioExtensions",
			"UMG",
			"GameplayTags",
			"GameplayAbilities",
			"GameplayCameras",
			"CinematicCamera",
			"GameplayTasks", 
			"SmartObjectsModule",
			"GameplayBehaviorsModule",
			"GameplayBehaviorSmartObjectsModule",
			"AIModule",
			"AnimGraphRuntime",
			"RigVM",
			"ControlRig",
			"EnhancedInput",
			"ALS",
			"ALSCamera",
			"Niagara",
			"NetCore",
			"NetCommon",
			"Networking",
			"GameFeatures",
			"ModularGameplay",
			"ModularGameplayActors",
			"AbilitySystemGameFeatureActions",
			"Chooser",
			"ContextualAnimation",
			"PhysicsControl",
			"DataRegistry",
			"TargetingSystem"
#if UE_5_5_OR_LATER
			// pass
#else
			"StructUtils",
#endif // UE_5_5_OR_LATER
		]);			
		
		PrivateDependencyModuleNames.AddRange([
			"EngineSettings",
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
