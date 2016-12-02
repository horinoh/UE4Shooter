// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/LocalPlayer.h"
#include "ShooterLocalPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API UShooterLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()
	
public:
	
	//!< ULocalPlayer
	virtual void SetControllerId(int32 NewControllerId) override;
	virtual FString GetNickname() const override;

	class UShooterSaveGame* GetSaveGame() const { return SaveGame; }

protected:
	UPROPERTY()
	class UShooterSaveGame* SaveGame;
};
