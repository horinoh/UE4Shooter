// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterWeapon_Projectile.h"

#include "AIController.h"

void AShooterWeapon_Projectile::Fire()
{
	FVector Start;
	FVector Direction;
	if (nullptr != Instigator)
	{
		const auto PC = Cast<APlayerController>(Instigator->GetController());
		if (nullptr != PC)
		{
			FRotator Rot;
			PC->GetPlayerViewPoint(Start, Rot);
			Direction = Rot.Vector();
		}
		else
		{
			Start = GetMuzzleLocation();
			const auto AC = Cast<AAIController>(Instigator->GetController());
			if (nullptr != AC)
			{
				Direction = AC->GetControlRotation().Vector();
			}
			else
			{
				Direction = Instigator->GetBaseAimRotation().Vector();
			}
		}
	}
	const auto End = Start + 10000.0f * Direction;

	//FHitResult HitResult(ForceInit);
	//if (TraceFire(Start, End, HitResult))
	//{
	//	ServerSpawnProjectile(Origin, (HitResult.ImpactPoint - Origin).GetSafeNormal());
	//}
	//else
	//{
	ServerSpawnProjectile(GetMuzzleLocation(), Direction);
	//}
}

bool AShooterWeapon_Projectile::ServerSpawnProjectile_Validate(FVector Location, FVector_NetQuantizeNormal Direction)
{
	return true;
}
void AShooterWeapon_Projectile::ServerSpawnProjectile_Implementation(FVector Location, FVector_NetQuantizeNormal Direction)
{
	if (nullptr != ProjectileClass)
	{
		const auto Transform = FTransform(Direction.Rotation(), Location);

		auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, Transform);
		if (nullptr != Projectile)
		{
			Projectile->Instigator = Instigator;
			Projectile->SetOwner(this);

			UGameplayStatics::FinishSpawningActor(Projectile, Transform);
		}
	}
	//else
	//{
	//	UE_LOG(LogShooter, Warning, TEXT("ProjectileClass is nullptr"));
	//}
}
