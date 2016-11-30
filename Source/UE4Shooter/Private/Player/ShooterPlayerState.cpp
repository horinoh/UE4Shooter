// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterPlayerState.h"

#include "Pawn/ShooterCharacter.h"

void AShooterPlayerState::ClientInitialize(class AController* C)
{
	Super::ClientInitialize(C);

	UpdateTeamColors();
}

void AShooterPlayerState::UpdateTeamColors()
{
	auto Controller = Cast<AController>(GetOwner());
	if (nullptr != Controller)
	{
		const auto Chara = Cast<AShooterCharacter>(Controller->GetCharacter());
		if (nullptr != Chara)
		{
			Chara->UpdateTeamColors();
		}
	}
}

void AShooterPlayerState::OnRep_TeamNumber()
{
	UpdateTeamColors();
}

void AShooterPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterPlayerState, TeamNumber);
}