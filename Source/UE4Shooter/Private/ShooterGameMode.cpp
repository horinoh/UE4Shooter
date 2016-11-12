// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterGameMode.h"

#include "Pawn/ShooterCharacter.h"
#include "Player/ShooterPlayerController.h"
#include "UI/ShooterHUD.h"

AShooterGameMode::AShooterGameMode()
	: Super()
{
	DefaultPawnClass = AShooterCharacter::StaticClass();
	PlayerControllerClass = AShooterPlayerController::StaticClass();
	HUDClass = AShooterHUD::StaticClass();
}
