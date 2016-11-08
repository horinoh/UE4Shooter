// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ShooterWeapon.generated.h"

UCLASS(Abstract)
class UE4SHOOTER_API AShooterWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AShooterWeapon(const FObjectInitializer& ObjectInitializer);
};
