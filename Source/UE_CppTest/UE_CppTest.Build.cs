// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE_CppTest : ModuleRules
{
	public UE_CppTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
