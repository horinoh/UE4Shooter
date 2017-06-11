// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class UE4ShooterServerTarget : TargetRules
{
	public UE4ShooterServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;

		ExtraModuleNames.Add("UE4Shooter");

		bUsesSteam = true;
	}
}
