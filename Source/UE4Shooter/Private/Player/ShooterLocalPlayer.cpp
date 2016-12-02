// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterLocalPlayer.h"

void UShooterLocalPlayer::SetControllerId(int32 NewControllerId)
{
	Super::SetControllerId(NewControllerId);
}

FString UShooterLocalPlayer::GetNickname() const
{
	return Super::GetNickname();
}
