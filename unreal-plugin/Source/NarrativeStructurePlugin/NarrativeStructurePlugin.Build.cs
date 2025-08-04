// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NarrativeStructurePlugin : ModuleRules
{
	public NarrativeStructurePlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",        // for UObject, AssetData, etc.
				"Engine",
				"Slate",              // for all Slate widgets
				"SlateCore",          // for their underlying types
				"Json",
				"JsonUtilities",
				"PropertyEditor",
				"UnrealEd",           // editor‐only bits
				"AssetRegistry",      // for FAssetRegistryModule
				"ContentBrowser" 
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"EditorFramework",
				"ToolMenus",
				"AssetTools", "EditorStyle",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
