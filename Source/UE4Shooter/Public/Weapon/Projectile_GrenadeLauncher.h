// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/ShooterProjectile.h"
#include "Projectile_GrenadeLauncher.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API AProjectile_GrenadeLauncher : public AShooterProjectile
{
	GENERATED_BODY()
	
public:
	AProjectile_GrenadeLauncher(const FObjectInitializer& ObjectInitializer);
};
