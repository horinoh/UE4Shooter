// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup/ShooterPickup.h"
#include "PickupGrenadeLauncher.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API APickupGrenadeLauncher : public AShooterPickup
{
	GENERATED_BODY()
	
public:
	APickupGrenadeLauncher(const FObjectInitializer& ObjectInitializer);

	//!< AShooterPickup
	virtual bool GiveTo(AActor* OtherActor) override;
};
