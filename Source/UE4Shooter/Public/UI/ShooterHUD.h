// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "ShooterHUD.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API AShooterHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	//!< AHUD
	virtual void DrawHUD() override;
};
