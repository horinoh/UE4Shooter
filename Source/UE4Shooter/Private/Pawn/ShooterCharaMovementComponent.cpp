// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterCharaMovementComponent.h"

#include "ShooterCharacter.h"

float UShooterCharaMovementComponent::GetMaxSpeed() const
{
	const auto Owner = Cast<AShooterCharacter>(GetCharacterOwner());
	if (nullptr != Owner)
	{
		return Super::GetMaxSpeed() * Owner->GetSpeedScale();
	}
	return Super::GetMaxSpeed();
}
float UShooterCharaMovementComponent::GetMaxAcceleration() const
{
	const auto Owner = Cast<AShooterCharacter>(GetCharacterOwner());
	if (nullptr != Owner)
	{
		return Super::GetMaxAcceleration() * Owner->GetSpeedScale();
	}
	return Super::GetMaxAcceleration();
}
