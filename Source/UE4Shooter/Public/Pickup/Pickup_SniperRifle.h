// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup/ShooterPickup.h"
#include "Pickup_SniperRifle.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API APickup_SniperRifle : public AShooterPickup
{
	GENERATED_BODY()

public:
	APickup_SniperRifle(const FObjectInitializer& ObjectInitializer);

	//!< AShooterPickup
	virtual bool GiveTo(AActor* OtherActor) override;
};
