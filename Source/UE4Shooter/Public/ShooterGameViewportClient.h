// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameViewportClient.h"
#include "ShooterGameViewportClient.generated.h"

/**
 * 
 */
UCLASS(Within = Engine, transient, config = Engine)
class UE4SHOOTER_API UShooterGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
	//!< UGameViewportClient
	virtual void NotifyPlayerAdded(int32 PlayerIndex, class ULocalPlayer* AddedPlayer) override;
};
