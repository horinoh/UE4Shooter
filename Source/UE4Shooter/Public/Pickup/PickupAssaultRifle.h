// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup/ShooterPickup_Ammo.h"
#include "PickupAssaultRifle.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API APickupAssaultRifle : public AShooterPickup_Ammo
{
	GENERATED_BODY()
	
public:
	APickupAssaultRifle(const FObjectInitializer& ObjectInitializer);
};
