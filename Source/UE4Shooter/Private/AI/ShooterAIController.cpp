// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Pawn/BotCharacter.h"

AShooterAIController::AShooterAIController()
{
	bWantsPlayerState = true;

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
}

void AShooterAIController::BeginInactiveState()
{
	Super::BeginInactiveState();
	
	const auto Delay = [&]()
	{
		const auto World = GetWorld();
		if (nullptr != World)
		{
			const auto GameState = World->GetGameState();
			if (nullptr != GameState)
			{
				return GameState->GetPlayerRespawnDelay(this);
			}
		}
		return 1.0f;
	}();
	GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &AShooterAIController::Respawn, Delay);
}
void AShooterAIController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	StopMovement();

	if (nullptr != BehaviorTreeComp)
	{
		BehaviorTreeComp->StopTree();
	}

	GetWorldTimerManager().ClearTimer(TimerHandle_Respawn);
}
void AShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
//!< #MY_TODO
//const auto Chara = Cast<ABotCharacter>(InPawn);
//if (nullptr != Chara)
//{
//	const auto BT = Chara->GetBehaviorTree();
//	if (nullptr != BT)
//	{
//		if (nullptr != BlackboardComp)
//		{
//			BlackboardComp->InitializeBlackboard(*BT->BlackboardAsset);
//		}
//		if (nullptr != BehaviorTreeComp)
//		{
//			BehaviorTreeComp->StartTree(*BT);
//		}
//	}
//}
}
void AShooterAIController::OnUnPossess()
{
	Super::OnUnPossess();

	if (nullptr != BehaviorTreeComp)
	{
		BehaviorTreeComp->StopTree();
	}
}

void AShooterAIController::Respawn()
{
	const auto World = GetWorld();
	if (nullptr != World)
	{
		const auto GameMode = World->GetAuthGameMode();
		if (nullptr != GameMode)
		{
			GameMode->RestartPlayer(this);
		}
	}
}
