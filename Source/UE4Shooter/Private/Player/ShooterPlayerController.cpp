// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterPlayerController.h"

#include "ShooterPlayerCameraManager.h"

AShooterPlayerController::AShooterPlayerController()
	: Super()
{
	PlayerCameraManagerClass = AShooterPlayerCameraManager::StaticClass();
}

void AShooterPlayerController::BeginInactiveState()
{
	Super::BeginInactiveState();

	const auto Delay = GetMinRespawnDelay();
	GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &AShooterPlayerController::Respawn, Delay);
}

void AShooterPlayerController::Respawn()
{
	const auto World = GetWorld();
	if (nullptr != World)
	{
		const auto GameMode = World->GetAuthGameMode();
		if (nullptr != GameMode)
		{
			GameMode->RestartPlayer(this);
		}
	}
}
