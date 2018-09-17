// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/ShooterWeapon_Projectile.h"
#include "WeaponRocketLauncher.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API AWeaponRocketLauncher : public AShooterWeapon_Projectile
{
	GENERATED_BODY()

public:
	AWeaponRocketLauncher();

	//!< AActor
	virtual void PostInitializeComponents() override;

	//!< AShooterWeapon
	FORCEINLINE virtual int32 GetAmmoPerClip() const override { return 1; }
	FORCEINLINE virtual int32 GetAmmoMax() const override { return 6 * GetAmmoPerClip(); }
	virtual void UnEquip() override;
	virtual void OnEquipFinished() override;
	virtual void StartSimulateFire() override;
	virtual float StartSimulateReload() override;

	void SetHideAmmo(const bool IsHide);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StaticMesh)
	UStaticMeshComponent* StaticMeshComp;
};
