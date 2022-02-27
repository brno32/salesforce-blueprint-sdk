// Copyright 2022 brno32. All Rights Reserved.

using UnrealBuildTool;

public class SalesforceBlueprintSDK : ModuleRules
{
	public SalesforceBlueprintSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Http",
				"Json",
				"JsonUtilities"
			}
		);
	}
}
