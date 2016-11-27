// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "PickupAssaultRifle.h"

#include "Pawn/ShooterCharacter.h"
#include "Weapon/WeaponAssaultRifle.h"

APickupAssaultRifle::APickupAssaultRifle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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
}

bool APickupAssaultRifle::GiveTo(AActor* OtherActor)
{
	const auto Chara = Cast<AShooterCharacter>(OtherActor);
	if (nullptr != Chara)
	{
		auto Weapon = Cast<AWeaponAssaultRifle>(Chara->GetWeapon());
		if (nullptr != Weapon)
		{
			return GiveAmmoTo(Weapon);
		}
	}
	return false;
}
