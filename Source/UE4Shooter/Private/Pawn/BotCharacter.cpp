// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "BotCharacter.h"

#include "AI/ShooterAIController.h"
#include "Perception/PawnSensingComponent.h"

ABotCharacter::ABotCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIControllerClass = AShooterAIController::StaticClass();

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	if (nullptr != PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &ABotCharacter::OnPawnSeen);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &ABotCharacter::OnNoiseHeard);
	}
}

void ABotCharacter::OnPawnSeen(APawn* SeenPawn)
{
	if (nullptr != SeenPawn)
	{
		DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);
	}
}

void ABotCharacter::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Yellow, false, 10.0f);

	auto Direction = Location - GetActorLocation();
	Direction.Normalize();

	auto Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	Rotation.Pitch = 0.0f;
	Rotation.Roll = 0.0f;

	SetActorRotation(Rotation);
}
