// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterWeapon_Projectile.h"

void AShooterWeapon_Projectile::Fire()
{
	const auto RandomSeed = FMath::Rand();
	auto RandomStream = FRandomStream(RandomSeed);

	const auto SpreadAngle = GetSpreadAngle();
	const auto ConeHalfRadian = FMath::DegreesToRadians(SpreadAngle * 0.5f);
	const auto RangeDistance = GetRangeDistance();

	FVector Start, Direction;
	GetAim(Start, Direction);
	const auto MuzzleLocation = GetMuzzleLocation();

	const auto SpreadDirection = RandomStream.VRandCone(Direction, ConeHalfRadian, ConeHalfRadian);
	const auto End = Start + RangeDistance * SpreadDirection;
	FHitResult HitResult(ForceInit);
	if (LineTraceWeapon(Start, End, HitResult))
	{
		ServerSpawnProjectile(MuzzleLocation, (HitResult.ImpactPoint - MuzzleLocation).GetSafeNormal());
	}
	else
	{
		ServerSpawnProjectile(MuzzleLocation, Direction);
	}
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
}
