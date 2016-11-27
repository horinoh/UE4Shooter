// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup/ShooterPickup.h"
#include "PickupShotgun.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API APickupShotgun : public AShooterPickup
{
	GENERATED_BODY()
	
public:
	APickupShotgun(const FObjectInitializer& ObjectInitializer);

	//!< AShooterPickup
	virtual bool GiveTo(AActor* OtherActor) override;
};
