// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "PickupAssaultRifle.h"

#include "Weapon/WeaponAssaultRifle.h"

APickupAssaultRifle::APickupAssaultRifle()
{
	if (nullptr != StaticMeshComp)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/PrototypeWeap/Prototype_AssaultRifle_Pickup.Prototype_AssaultRifle_Pickup'"));
		if (SM.Succeeded())
		{
			StaticMeshComp->SetStaticMesh(SM.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/Rifle/Rifle_AmmoPickup_Cue.Rifle_AmmoPickup_Cue'"));
	if (SC.Succeeded())
	{
		PickupSoundCue = SC.Object;
	}

	WeaponType = AWeaponAssaultRifle::StaticClass();
}
