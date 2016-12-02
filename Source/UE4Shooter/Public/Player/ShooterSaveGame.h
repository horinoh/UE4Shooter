// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "ShooterSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API UShooterSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	void Save();
	static UShooterSaveGame* Load(const FString& SlotName, const int32 UserIndex);
	void TellInputAboutKeybindings();

	FORCEINLINE float GetAimSensitivity() const { return AimSensitivity; }
	FORCEINLINE void SetAimSensitivity(float val) { AimSensitivity = val; }

	FORCEINLINE bool GetInvertedYAxis() const { return bInvertedYAxis; }
	FORCEINLINE void SetInvertedYAxis(bool val) { bInvertedYAxis = val; }

protected:
	UPROPERTY()
	float AimSensitivity;
	UPROPERTY()
	bool bInvertedYAxis;
	
	UPROPERTY()
	int32 Kills;
	UPROPERTY()
	int32 Deaths;

private:
	FString SlotName;
	int32 UserIndex;
};
