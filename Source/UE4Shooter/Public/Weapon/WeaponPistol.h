// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/ShooterWeapon_Instant.h"
#include "WeaponPistol.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API AWeaponPistol : public AShooterWeapon_Instant
{
	GENERATED_BODY()

	AWeaponPistol(const FObjectInitializer& ObjectInitializer);
};
