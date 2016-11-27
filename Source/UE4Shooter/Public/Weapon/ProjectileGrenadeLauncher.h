// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/ShooterProjectile.h"
#include "ProjectileGrenadeLauncher.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API AProjectileGrenadeLauncher : public AShooterProjectile
{
	GENERATED_BODY()
	
public:
	AProjectileGrenadeLauncher(const FObjectInitializer& ObjectInitializer);
};
