// MIT

using System.Linq;
using UnrealBuildTool;

public class ALSXT : ModuleRules
{
	public ALSXT(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		
		// Toggle optimization based on a custom flag
		// To Activate, run build with: -define:WITH_COVERAGE=1
		if (Target.GlobalDefinitions.Contains("WITH_COVERAGE=1"))
		{
			OptimizeCode = CodeOptimization.Never;
			bUseUnity = false;
			
			if (Target.Platform == UnrealTargetPlatform.Win64)
			{
				// /Ob0: Disables inline expansion
				// /Od: Disables optimizations (optional, but often needed with /Ob0)
				// PrivateCompilerFlags.Add("/Ob0"); 
			}
			
			if (Target.Platform == UnrealTargetPlatform.Mac || Target.Platform == UnrealTargetPlatform.Linux)
			{
				// PrivateCompilerFlags.Add("-fno-inline");
			}
		}

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
			"SlateCore"
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
