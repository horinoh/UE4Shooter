// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterGameViewportClient.h"

#include "Player/ShooterLocalPlayer.h"

void UShooterGameViewportClient::NotifyPlayerAdded(int32 PlayerIndex, class ULocalPlayer* AddedPlayer)
{
	Super::NotifyPlayerAdded(PlayerIndex, AddedPlayer);

	const auto LP = Cast<UShooterLocalPlayer>(AddedPlayer);
	if (nullptr != LP)
	{
		LP->LoadSaveGame();
	}
}
