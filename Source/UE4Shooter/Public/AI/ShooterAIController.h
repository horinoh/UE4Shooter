// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API AShooterAIController : public AAIController
{
	GENERATED_BODY()
	
public:	
	AShooterAIController();

	//!< AController
	virtual void BeginInactiveState() override;
	virtual void GameHasEnded(class AActor* EndGameFocus = NULL, bool bIsWinner = false) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	void Respawn();

protected:
	FTimerHandle TimerHandle_Respawn;

	UPROPERTY(Transient)
	UBlackboardComponent* BlackboardComp;
	UPROPERTY(Transient)
	UBehaviorTreeComponent* BehaviorTreeComp;
};
