// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "PickupRocketLauncher.h"

#include "Pawn/ShooterCharacter.h"
#include "Weapon/WeaponRocketLauncher.h"

APickupRocketLauncher::APickupRocketLauncher(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (nullptr != StaticMeshComp)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/PrototypeWeap/Prototype_RocketLauncher_Pickup.Prototype_RocketLauncher_Pickup'"));
		if (SM.Succeeded())
		{
			StaticMeshComp->SetStaticMesh(SM.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/RocketLauncher/RL_AmmoPickup_Cue.RL_AmmoPickup_Cue'"));
	if (SC.Succeeded())
	{
		PickupSoundCue = SC.Object;
	}
}

bool APickupRocketLauncher::GiveTo(AActor* OtherActor)
{
	const auto Chara = Cast<AShooterCharacter>(OtherActor);
	if (nullptr != Chara)
	{
		auto Weapon = Cast<AWeaponRocketLauncher>(Chara->GetWeapon());
		if (nullptr != Weapon)
		{
			return GiveAmmoTo(Weapon);
		}
	}
	return false;
}
