// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterPlayerCameraManager.h"

#include "Pawn/ShooterCharacter.h"
#include "Weapon/ShooterWeapon.h"

AShooterPlayerCameraManager::AShooterPlayerCameraManager()
	: Super()
{
	bAlwaysApplyModifiers = true;
}

void AShooterPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	if (nullptr != PCOwner)
	{
		const auto Pawn = Cast<AShooterCharacter>(PCOwner->GetPawn());
		if (nullptr != Pawn)
		{
			const auto Weapon = Cast<AShooterWeapon>(Pawn->GetWeapon());
			if (nullptr != Weapon)
			{
				const auto TargetingFOV = Pawn->IsTargeting() ? Weapon->GetTargetingFOV() : DefaultFOV;
				CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetingFOV, DeltaTime, 20.0f);
				SetFOV(CurrentFOV);
			}
		}
	}
}
