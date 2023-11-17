// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SmultronBarna : ModuleRules
{
	public SmultronBarna(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				"SmultronBarna"
			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {
			}
		);
        
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"NavigationSystem",
			"AIModule",
			"GameplayTags",
			"ModularGameplay",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"EnhancedInput",
            "UMG"
		});
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
			}
		);
		
		// Generate compile errors if using DrawDebug functions in test/shipping builds.
		PublicDefinitions.Add("SHIPPING_DRAW_DEBUG_ERROR=1");
		

		SetupGameplayDebuggerSupport(Target);
		SetupIrisSupport(Target);
	}
}
