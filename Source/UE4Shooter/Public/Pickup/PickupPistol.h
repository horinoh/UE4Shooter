// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup/ShooterPickup_Ammo.h"
#include "PickupPistol.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API APickupPistol : public AShooterPickup_Ammo
{
	GENERATED_BODY()
	
public:
	APickupPistol(const FObjectInitializer& ObjectInitializer);
};
