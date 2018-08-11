// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pawn/ShooterCharacter.h"
#include "BotCharacter.generated.h"

class UPawnSensingComponent;

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API ABotCharacter : public AShooterCharacter
{
	GENERATED_BODY()
	
public:
	ABotCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);
	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

protected:
	UPROPERTY(VisibleAnywhere, Category = Sensing)
	UPawnSensingComponent* PawnSensingComp;
};
