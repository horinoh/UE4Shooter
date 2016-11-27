// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "PickupPistol.h"

#include "Pawn/ShooterCharacter.h"
#include "Weapon/WeaponPistol.h"

APickupPistol::APickupPistol(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (nullptr != StaticMeshComp)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/PrototypeWeap/Prototype_Pistol_Pickup.Prototype_Pistol_Pickup'"));
		if (SM.Succeeded())
		{
			StaticMeshComp->SetStaticMesh(SM.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/Pistol/Pistol_AmmoPickup_Cue.Pistol_AmmoPickup_Cue'"));
	if (SC.Succeeded())
	{
		PickupSoundCue = SC.Object;
	}
}

bool APickupPistol::GiveTo(AActor* OtherActor)
{
	const auto Chara = Cast<AShooterCharacter>(OtherActor);
	if (nullptr != Chara)
	{
		auto Weapon = Cast<AWeaponPistol>(Chara->GetWeapon());
		if (nullptr != Weapon)
		{
			return GiveAmmoTo(Weapon);
		}
	}
	return false;
}
