// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterGameMode.h"

#include "Pawn/ShooterCharacter.h"
#include "Player/ShooterPlayerController.h"
#include "Player/ShooterPlayerState.h"
#include "UI/ShooterHUD.h"

AShooterGameMode::AShooterGameMode()
	: Super()
{
	DefaultPawnClass = AShooterCharacter::StaticClass();
	PlayerControllerClass = AShooterPlayerController::StaticClass();
	PlayerStateClass = AShooterPlayerState::StaticClass();
	HUDClass = AShooterHUD::StaticClass();
}

void AShooterGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (nullptr != NewPlayer)
	{
		auto PS = CastChecked<AShooterPlayerState>(NewPlayer->PlayerState);
		if (nullptr != PS)
		{
			//!< #MY_TODO ƒ`[ƒ€•ª‚¯
			PS->SetTeamNumber(FMath::Rand()%4);
		}
	}
}
