// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup/ShooterPickup.h"
#include "Pickup_Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API APickup_Shotgun : public AShooterPickup
{
	GENERATED_BODY()
	
public:
	APickup_Shotgun(const FObjectInitializer& ObjectInitializer);

	//!< AShooterPickup
	virtual bool GiveTo(AActor* OtherActor) override;
};
