// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "PickupShotgun.h"

#include "Pawn/ShooterCharacter.h"
#include "Weapon/WeaponShotgun.h"

APickupShotgun::APickupShotgun(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (nullptr != StaticMeshComp)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/PrototypeWeap/Prototype_Shotgun_Pickup.Prototype_Shotgun_Pickup'"));
		if (SM.Succeeded())
		{
			StaticMeshComp->SetStaticMesh(SM.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/Shotgun/Shotgun_AmmoPickup_Cue.Shotgun_AmmoPickup_Cue'"));
	if (SC.Succeeded())
	{
		PickupSoundCue = SC.Object;
	}
}

bool APickupShotgun::GiveTo(AActor* OtherActor)
{
	const auto Chara = Cast<AShooterCharacter>(OtherActor);
	if (nullptr != Chara)
	{
		auto Weapon = Cast<AWeaponShotgun>(Chara->GetWeapon());
		if (nullptr != Weapon)
		{
			return GiveAmmoTo(Weapon);
		}
	}
	return false;
}
