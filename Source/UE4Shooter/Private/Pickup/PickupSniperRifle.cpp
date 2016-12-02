// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "PickupSniperRifle.h"

#include "Weapon/WeaponSniperRifle.h"

APickupSniperRifle::APickupSniperRifle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (nullptr != StaticMeshComp)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/PrototypeWeap/Prototype_SniperRifle_Pickup.Prototype_SniperRifle_Pickup'"));
		if (SM.Succeeded())
		{
			StaticMeshComp->SetStaticMesh(SM.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/SniperRifle/SR_AmmoPickup01_Cue.SR_AmmoPickup01_Cue'")); //!< 02, 03 ‚à‚ ‚é
	if (SC.Succeeded())
	{
		PickupSoundCue = SC.Object;
	}

	WeaponType = AWeaponSniperRifle::StaticClass();
}
