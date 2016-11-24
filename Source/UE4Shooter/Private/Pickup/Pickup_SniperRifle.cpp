// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "Pickup_SniperRifle.h"

#include "Pawn/ShooterCharacter.h"
#include "Weapon/WeaponSniperRifle.h"

APickup_SniperRifle::APickup_SniperRifle(const FObjectInitializer& ObjectInitializer)
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

	static ConstructorHelpers::FObjectFinder<USoundCue> SC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/SniperRifle/SR_AmmoPickup01_Cue.SR_AmmoPickup01_Cue'")); //!< 02, 03 ������
	if (SC.Succeeded())
	{
		PickupSoundCue = SC.Object;
	}
}

bool APickup_SniperRifle::GiveTo(AActor* OtherActor)
{
	const auto Chara = Cast<AShooterCharacter>(OtherActor);
	if (nullptr != Chara)
	{
		auto Weapon = Cast<AWeaponSniperRifle>(Chara->GetWeapon());
		if (nullptr != Weapon)
		{
			return GiveAmmoTo(Weapon);
		}
	}
	return false;
}
