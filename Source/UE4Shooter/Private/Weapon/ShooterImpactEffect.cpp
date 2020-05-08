// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterImpactEffect.h"

AShooterImpactEffect::AShooterImpactEffect()
{
	SetAutoDestroyWhenFinished(true);
}

void AShooterImpactEffect::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const auto PhysSurface = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());

	const auto Location = HitResult.ImpactPoint;
	const auto Rotation = HitResult.ImpactNormal.Rotation();

	//!< パーティクル
	const auto Particle = GetParticle(PhysSurface);
	if (nullptr != Particle)
	{
		const auto ParticleComp = UGameplayStatics::SpawnEmitterAtLocation(this, Particle, Location, Rotation);
	}
	else
	{
		DrawDebugSphere(GetWorld(), Location, 5.0f, 8, FColor::Red, false, 2.0f);
	}

	//!< サウンド
	const auto Sound = GetSound(PhysSurface);
	if (nullptr != Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sound, Location);
	}

	//!< デカール
	const auto Decal = GetDecal(PhysSurface);
	if (nullptr != Decal)
	{
		auto DecalRotation = Rotation;
		DecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);

		const auto Size = FVector(32.0f, 32.0f, 1.0f);
		const auto LifeSpan = 50.0f;
		const auto DecalComp = UGameplayStatics::SpawnDecalAttached(Decal, Size, HitResult.Component.Get(), HitResult.BoneName, Location, DecalRotation, EAttachLocation::KeepWorldPosition, LifeSpan);
	}
	else
	{
		DrawDebugBox(GetWorld(), Location, FVector(8.0f, 8.0f, 1.0f), FQuat::FindBetweenNormals(HitResult.ImpactNormal, FVector::UpVector), FColor::White, false, 50.0f);
	}
}

UParticleSystem* AShooterImpactEffect::GetParticle(const TEnumAsByte<EPhysicalSurface> PhysSurface) const
{
	switch (PhysSurface)
	{
	default:
	case EPhysicalSurface::SurfaceType_Default:
		return ImpactParticle;
		break;
	}
	return nullptr;
}
USoundCue* AShooterImpactEffect::GetSound(const TEnumAsByte<EPhysicalSurface> PhysSurface) const
{
	switch (PhysSurface)
	{
	default:
	case EPhysicalSurface::SurfaceType_Default:
		return ImpactSoundCue;
		break;
	}
	return nullptr;
}
UMaterialInterface* AShooterImpactEffect::GetDecal(const TEnumAsByte<EPhysicalSurface> PhysSurface) const
{
	switch (PhysSurface)
	{
	default:
	case EPhysicalSurface::SurfaceType_Default:
		return ImpactDecal;
		break;
	}
	return nullptr;
}

