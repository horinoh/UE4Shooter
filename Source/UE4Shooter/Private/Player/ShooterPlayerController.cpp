// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterPlayerController.h"

#include "ShooterPlayerCameraManager.h"
#include "ShooterLocalPlayer.h"
#include "ShooterSaveGame.h"

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

void AShooterPlayerController::InitInputSystem()
{
	Super::InitInputSystem();

	const auto LP = Cast<UShooterLocalPlayer>(Player);
	if (nullptr != LP)
	{
		const auto SaveGame = LP->GetSaveGame();
		if (nullptr != SaveGame)
		{
			SaveGame->TellInputAboutKeybindings();
		}
	}
}
void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	//!< セーブ
	const auto LP = Cast<UShooterLocalPlayer>(Player);
	if (nullptr != LP)
	{
		const auto SaveGame = LP->GetSaveGame();
		if (nullptr != SaveGame)
		{
			SaveGame->SaveIfDirty();
		}
	}
}
void AShooterPlayerController::ClientGameEnded_Implementation(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::ClientGameEnded_Implementation(EndGameFocus, bIsWinner);

	SetIgnoreMoveInput(true);
	SetViewTarget(GetPawn());

	//!< セーブ
	const auto LP = Cast<UShooterLocalPlayer>(Player);
	if (nullptr != LP)
	{
		const auto SaveGame = LP->GetSaveGame();
		if (nullptr != SaveGame)
		{
			SaveGame->SaveIfDirty();
		}
	}
}
