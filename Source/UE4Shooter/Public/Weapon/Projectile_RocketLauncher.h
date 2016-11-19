// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/ShooterProjectile.h"
#include "Projectile_RocketLauncher.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API AProjectile_RocketLauncher : public AShooterProjectile
{
	GENERATED_BODY()
	
public:
	AProjectile_RocketLauncher(const FObjectInitializer& ObjectInitializer);
};
