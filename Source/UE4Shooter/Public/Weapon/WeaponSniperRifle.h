// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/ShooterWeapon_Instant.h"
#include "WeaponSniperRifle.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API AWeaponSniperRifle : public AShooterWeapon_Instant
{
	GENERATED_BODY()

	AWeaponSniperRifle(const FObjectInitializer& FObjectInitializer);

	//!< AShooterWeapon
	FORCEINLINE virtual float GetTargetingFOV() const { return 30.0f; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue* ZoomInSoundCue;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue* ZoomOutSoundCue;
};
