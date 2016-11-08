// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/ShooterWeapon_Projectile.h"
#include "WeaponRocketLauncher.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API AWeaponRocketLauncher : public AShooterWeapon_Projectile
{
	GENERATED_BODY()

	AWeaponRocketLauncher(const FObjectInitializer& ObjectInitializer);
};
