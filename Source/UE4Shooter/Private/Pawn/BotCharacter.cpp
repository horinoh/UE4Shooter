// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "BotCharacter.h"

#include "AI/ShooterAIController.h"

ABotCharacter::ABotCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIControllerClass = AShooterAIController::StaticClass();
}
