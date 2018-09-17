// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DamageType.h"
#include "ShooterDamageType.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API UShooterDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UShooterDamageType();

	//UPROPERTY(EditDefaultsOnly, Category = HUD)
	//FCanvasIcon KillIcon;
		
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UForceFeedbackEffect *HitForceFeedbackEffect;
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UForceFeedbackEffect *KilledForceFeedbackEffect;
};
