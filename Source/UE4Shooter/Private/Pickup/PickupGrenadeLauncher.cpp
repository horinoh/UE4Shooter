// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "PickupGrenadeLauncher.h"

#include "Pawn/ShooterCharacter.h"
#include "Weapon/WeaponGrenadeLauncher.h"

APickupGrenadeLauncher::APickupGrenadeLauncher(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (nullptr != StaticMeshComp)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/PrototypeWeap/Prototype_GrenadeLauncher_Pickup.Prototype_GrenadeLauncher_Pickup'"));
		if (SM.Succeeded())
		{
			StaticMeshComp->SetStaticMesh(SM.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/GrenadeLauncher/GL_AmmoPickup_Cue.GL_AmmoPickup_Cue'"));
	if (SC.Succeeded())
	{
		PickupSoundCue = SC.Object;
	}
}

bool APickupGrenadeLauncher::GiveTo(AActor* OtherActor)
{
	const auto Chara = Cast<AShooterCharacter>(OtherActor);
	if (nullptr != Chara)
	{
		auto Weapon = Cast<AWeaponGrenadeLauncher>(Chara->GetWeapon());
		if (nullptr != Weapon)
		{
			return GiveAmmoTo(Weapon);
		}
	}
	return false;
}
