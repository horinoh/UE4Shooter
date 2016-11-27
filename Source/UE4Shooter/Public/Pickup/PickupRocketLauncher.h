// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup/ShooterPickup.h"
#include "PickupRocketLauncher.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API APickupRocketLauncher : public AShooterPickup
{
	GENERATED_BODY()
	
public:
	APickupRocketLauncher(const FObjectInitializer& ObjectInitializer);

	//!< AShooterPickup
	virtual bool GiveTo(AActor* OtherActor) override;
};
