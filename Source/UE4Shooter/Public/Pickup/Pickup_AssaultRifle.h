// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup/ShooterPickup.h"
#include "Pickup_AssaultRifle.generated.h"

/**
 * 
 */
UCLASS()
class APickup_AssaultRifle : public AShooterPickup
{
	GENERATED_BODY()
	
public:
	APickup_AssaultRifle(const FObjectInitializer& ObjectInitializer);

	//!< AShooterPickup
	virtual bool GiveTo(AActor* OtherActor) override;
};
