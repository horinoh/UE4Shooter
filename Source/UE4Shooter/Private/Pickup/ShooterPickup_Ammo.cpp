// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterPickup_Ammo.h"

#include "Weapon/ShooterWeapon.h"
#include "Pawn/ShooterCharacter.h"

AShooterPickup_Ammo::AShooterPickup_Ammo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool AShooterPickup_Ammo::GiveTo(AActor* OtherActor)
{
	const auto Chara = Cast<AShooterCharacter>(OtherActor);
	if (nullptr != Chara)
	{
		const auto Weapon = Chara->GetWeapon();
		if (nullptr != Weapon && Weapon->IsA(WeaponType))
		{
			const auto AddAmount = Weapon->GiveAmmo(Weapon->GetAmmoMax());
			if (AddAmount)
			{
				DrawDebugString(GetWorld(), GetActorLocation(), TEXT("+ ") + FString::FromInt(AddAmount), nullptr, FColor::Yellow, 5.0f);
				return true;
			}
		}
	}
	return false;
}
