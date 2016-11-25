// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/ShooterProjectile.h"
#include "ProjectileRocketLauncher.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API AProjectileRocketLauncher : public AShooterProjectile
{
	GENERATED_BODY()
	
public:
	AProjectileRocketLauncher(const FObjectInitializer& ObjectInitializer);

	//!< AActor
	virtual void PostInitializeComponents() override;

	//!< AShooterProjectile
	virtual void SimulateExplode(const FHitResult& HitResult) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Sound)
	UAudioComponent* InAirSoundComp;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue* InAirSoundCue;
};
