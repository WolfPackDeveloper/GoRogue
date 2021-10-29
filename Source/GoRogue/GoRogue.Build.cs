// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GoRogue : ModuleRules
{
	public GoRogue(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "GameplayTasks", "UMG", "GameplayTags" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[]
		{
			"GoRogue/Public/AI",
			"GoRogue/Public/Characters",
			"GoRogue/Public/Components",
			"GoRogue/Public/Core",
			"GoRogue/Public/Environment",
			"GoRogue/Public/microGAS",
			"GoRogue/Public/Interfaces",
			"GoRogue/Public/Weapon",
			"GoRogue/Public/Widgets"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
