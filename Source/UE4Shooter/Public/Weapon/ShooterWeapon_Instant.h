// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/ShooterWeapon.h"
#include "ShooterWeapon_Instant.generated.h"

USTRUCT()
struct FFireNotify
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY()
		TArray<FVector> EndLocations;
};

/**
 * 
 */
UCLASS(Abstract)
class UE4SHOOTER_API AShooterWeapon_Instant : public AShooterWeapon
{
	GENERATED_BODY()

public:
	//!< AShooterWeapon
	virtual void Fire() override;
	virtual void OnEquipFinished() override;

	void HitConfirmed(const FHitResult& HitResult);

	void Hits(const TArray<FHitResult>& HitResults);
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerHits(const TArray<FHitResult>& HitResults);
	virtual bool ServerHits_Validate(const TArray<FHitResult> HitResults);
	virtual void ServerHits_Implementation(const TArray<FHitResult> HitResults);

	void Misses(const int32 RandomSeed, const uint32 MissResults);
	UFUNCTION(Unreliable, Server, WithValidation)
	void ServerMisses(const int32 RandomSeed, const uint32 MissResults);
	virtual bool ServerMisses_Validate(const int32 RandomSeed, const uint32 MissResults);
	virtual void ServerMisses_Implementation(const int32 RandomSeed, const uint32 MissResults);

	UFUNCTION()
	void OnRep_FireNotify();

	FORCEINLINE void SpawnImpactEffect(const FHitResult& HitResult) { AShooterWeapon::SpawnImpactEffect(GetWorld(), ImpactEffectClass, HitResult); }
	FORCEINLINE void SpawnTrailEffect(const FVector& End) { DrawDebugLine(GetWorld(), GetMuzzleLocation(), End, FColor::Red, false, 2.0f); }

protected:
	UPROPERTY(Transient, ReplicatedUsing = OnRep_FireNotify)
	FFireNotify FireNotify;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	TSubclassOf<class AShooterImpactEffect> ImpactEffectClass;
};
