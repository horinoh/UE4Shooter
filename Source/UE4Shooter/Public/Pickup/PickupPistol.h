// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup/ShooterPickup.h"
#include "PickupPistol.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API APickupPistol : public AShooterPickup
{
	GENERATED_BODY()
	
public:
	APickupPistol(const FObjectInitializer& ObjectInitializer);

	//!< AShooterPickup
	virtual bool GiveTo(AActor* OtherActor) override;
};
