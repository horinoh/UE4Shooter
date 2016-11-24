// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup/ShooterPickup.h"
#include "Pickup_GrenadeLauncher.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API APickup_GrenadeLauncher : public AShooterPickup
{
	GENERATED_BODY()
	
public:
	APickup_GrenadeLauncher(const FObjectInitializer& ObjectInitializer);

	//!< AShooterPickup
	virtual bool GiveTo(AActor* OtherActor) override;
};
