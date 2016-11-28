// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AShooterPlayerController();

	//!< APlayerController
	virtual void UnFreeze() override;
};
