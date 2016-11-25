// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ShooterImpactEffect.generated.h"

UCLASS()
class UE4SHOOTER_API AShooterImpactEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	AShooterImpactEffect();

	//!< AActor
	virtual void PostInitializeComponents() override;

	FORCEINLINE void SetHitResult(FHitResult val) { HitResult = val; }

	virtual UParticleSystem* GetParticle(const TEnumAsByte<EPhysicalSurface> PhysSurface) const;
	virtual USoundCue* GetSound(const TEnumAsByte<EPhysicalSurface> PhysSurface) const;
	virtual UMaterialInterface* GetDecal(const TEnumAsByte<EPhysicalSurface> PhysSurface) const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = HitResult)
	FHitResult HitResult;

	UPROPERTY(EditDefaultsOnly, Category = Particle)
	UParticleSystem* ImpactParticle;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue* ImpactSoundCue;

	UPROPERTY(EditDefaultsOnly, Category = Decal)
	UMaterialInterface* ImpactDecal;
};
