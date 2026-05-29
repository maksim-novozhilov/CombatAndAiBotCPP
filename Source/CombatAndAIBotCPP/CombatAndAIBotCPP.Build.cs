// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CombatAndAIBotCPP : ModuleRules
{
	public CombatAndAIBotCPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        //PublicIncludePaths.AddRange(new string[] { "CombatAndAIBotCPP" });

        PublicIncludePaths.AddRange(new string[] { System.IO.Path.Combine(ModuleDirectory) });


        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
