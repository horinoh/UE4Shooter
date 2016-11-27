// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup/ShooterPickup.h"
#include "PickupSniperRifle.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API APickupSniperRifle : public AShooterPickup
{
	GENERATED_BODY()
	
public:
	APickupSniperRifle(const FObjectInitializer& ObjectInitializer);

	//!< AShooterPickup
	virtual bool GiveTo(AActor* OtherActor) override;
};
