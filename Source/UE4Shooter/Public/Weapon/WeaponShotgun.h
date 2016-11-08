// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/ShooterWeapon_Instant.h"
#include "WeaponShotgun.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API AWeaponShotgun : public AShooterWeapon_Instant
{
	GENERATED_BODY()

	AWeaponShotgun(const FObjectInitializer& ObjectInitializer);
};
