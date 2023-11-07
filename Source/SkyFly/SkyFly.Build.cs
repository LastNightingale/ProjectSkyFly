// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SkyFly : ModuleRules
{
	public SkyFly(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", 
			"Slate", "SlateCore", "OnlineSubsystemSteam" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		PrivateIncludePaths.Add( "SkyFly" );

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
