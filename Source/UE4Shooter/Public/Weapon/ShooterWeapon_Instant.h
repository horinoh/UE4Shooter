// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/ShooterWeapon.h"
#include "ShooterWeapon_Instant.generated.h"

USTRUCT()
struct FFireNotify
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FVector Origin;
	
	UPROPERTY()
	int32 RandomSeed;
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

	void HitConfirmed(const FHitResult& HitResult);

	void Hits(const FVector Start, const int32 RandomSeed, const TArray<FHitResult>& HitResults);
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerHits(const FVector Start, const int32 RandomSeed, const TArray<FHitResult>& HitResults);
	virtual bool ServerHits_Validate(const FVector Start, const int32 RandomSeed, const TArray<FHitResult> HitResults);
	virtual void ServerHits_Implementation(const FVector Start, const int32 RandomSeed, const TArray<FHitResult> HitResults);

	UFUNCTION()
	void OnRep_FireNotify();

	FORCEINLINE void SpawnImpactEffect(const FHitResult& HitResult) { AShooterWeapon::SpawnImpactEffect(GetWorld(), ImpactEffectClass, HitResult); }
	void SpawnTrailEffect(const FVector& End);

protected:
	UPROPERTY(Transient, ReplicatedUsing = OnRep_FireNotify)
	FFireNotify FireNotify;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	TSubclassOf<class AShooterImpactEffect> ImpactEffectClass;
};
