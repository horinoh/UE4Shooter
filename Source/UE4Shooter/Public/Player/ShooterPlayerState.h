// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "ShooterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class AShooterPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	//!< APlayerState
	virtual void ClientInitialize(class AController* C) override;

	void UpdateTeamColors();

	UFUNCTION()
	void OnRep_TeamNumber();

	FORCEINLINE int32 GetTeamNumber() const { return TeamNumber; }
	FORCEINLINE void SetTeamNumber(int32 val) { TeamNumber = val; UpdateTeamColors(); }

protected:
	UPROPERTY(Transient, ReplicatedUsing = OnRep_TeamNumber)
	int32 TeamNumber;
};
