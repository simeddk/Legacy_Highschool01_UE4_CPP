// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class U06_ThirdPersonCPPTarget : TargetRules
{
	public U06_ThirdPersonCPPTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "U06_ThirdPersonCPP" } );
	}
}
