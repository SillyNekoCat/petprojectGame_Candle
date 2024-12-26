// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Living_Candle : ModuleRules
{
	public Living_Candle(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities", "GameplayTags" });
	}
}
