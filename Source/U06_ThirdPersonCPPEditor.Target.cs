// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class U06_ThirdPersonCPPEditorTarget : TargetRules
{
	public U06_ThirdPersonCPPEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "U06_ThirdPersonCPP" } );
	}
}
