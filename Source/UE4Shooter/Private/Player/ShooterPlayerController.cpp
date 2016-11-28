// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterPlayerController.h"

#include "ShooterPlayerCameraManager.h"

AShooterPlayerController::AShooterPlayerController()
	: Super()
{
	PlayerCameraManagerClass = AShooterPlayerCameraManager::StaticClass();
}

void AShooterPlayerController::UnFreeze()
{
	Super::UnFreeze(); 
	
	ServerRestartPlayer(); 
}
