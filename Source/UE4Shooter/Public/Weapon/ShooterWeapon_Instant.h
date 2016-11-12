// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/ShooterWeapon.h"
#include "ShooterWeapon_Instant.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UE4SHOOTER_API AShooterWeapon_Instant : public AShooterWeapon
{
	GENERATED_BODY()

	//!< AShooterWeapon
	virtual void Fire() override {}
};
