// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterDamageType.h"

UShooterDamageType::UShooterDamageType()
{
	static ConstructorHelpers::FObjectFinder<UForceFeedbackEffect> FFE_Hit(TEXT("ForceFeedbackEffect'/Game/Shooter/Forcefeedback/FFE_Hit.FFE_Hit'"));
	HitForceFeedbackEffect = FFE_Hit.Object;

	static ConstructorHelpers::FObjectFinder<UForceFeedbackEffect> FFE_Killed(TEXT("ForceFeedbackEffect'/Game/Shooter/Forcefeedback/FFE_Killed.FFE_Killed'"));
	KilledForceFeedbackEffect = FFE_Killed.Object;
}
