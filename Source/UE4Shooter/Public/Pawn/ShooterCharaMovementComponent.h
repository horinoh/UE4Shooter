// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterCharaMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class UShooterCharaMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxAcceleration() const override;
};
