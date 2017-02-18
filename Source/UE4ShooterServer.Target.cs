// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class UE4ShooterServerTarget : TargetRules
{
	public UE4ShooterServerTarget(TargetInfo Target)
	{
		Type = TargetType.Server;
		bUsesSteam = true;
	}

	//
	// TargetRules interface.
	//

	//public override bool GetSupportedPlatforms(ref List<UnrealTargetPlatform> OutPlatforms)
	//{
	//    return UnrealBuildTool.UnrealBuildTool.GetAllServerPlatforms(ref OutPlatforms, false);
	//}

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.AddRange( new string[] { "UE4Shooter" } );
	}
}
