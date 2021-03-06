// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "ShooterPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class AShooterPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	AShooterPlayerCameraManager();

	//!< APlayerCameraManager
	virtual void UpdateCamera(float DeltaTime) override;

protected:
	float CurrentFOV = DefaultFOV;
};
