// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup/ShooterPickup.h"
#include "ShooterPickup_Ammo.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API AShooterPickup_Ammo : public AShooterPickup
{
	GENERATED_BODY()

public:
	AShooterPickup_Ammo();

	//!< AShooterPickup
	virtual bool GiveTo(AActor* OtherActor) override;
};
