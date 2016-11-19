// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShooterProjectile.h"

#include "Weapon/ShooterWeapon.h"
#include "ShooterWeapon_Projectile.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UE4SHOOTER_API AShooterWeapon_Projectile : public AShooterWeapon
{
	GENERATED_BODY()

public:
	//!< AShooterWeapon
	virtual void Fire() override;

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSpawnProjectile(FVector Location, FVector_NetQuantizeNormal Direction);
	virtual bool ServerSpawnProjectile_Validate(FVector Location, FVector_NetQuantizeNormal Direction);
	virtual void ServerSpawnProjectile_Implementation(FVector Location, FVector_NetQuantizeNormal Direction);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<AShooterProjectile> ProjectileClass;
};
