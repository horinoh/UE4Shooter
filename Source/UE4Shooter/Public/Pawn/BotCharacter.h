// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pawn/ShooterCharacter.h"
#include "BotCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API ABotCharacter : public AShooterCharacter
{
	GENERATED_BODY()
	
public:
	ABotCharacter(const FObjectInitializer& ObjectInitializer);
};
