// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ShooterWeapon.generated.h"

UCLASS(Abstract)
class UE4SHOOTER_API AShooterWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AShooterWeapon();

	//!< AActor
	virtual void PostInitializeComponents() override;

	FORCEINLINE bool IsLocallyControlled() const 
	{ 
		const auto Pawn = Cast<APawn>(GetOwner());
		return nullptr != Pawn && Pawn->IsLocallyControlled();
	}

	FORCEINLINE bool IsEquipping() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_EquipFinished); }
	virtual void Equip(APawn* NewOwner);
	virtual void UnEquip();
	virtual void OnEquipFinished();

	FORCEINLINE UTexture2D* GetCrosshairTexture() const { return CrosshairTexture; }

	FORCEINLINE virtual float GetTargetingFOV() const { return 60.0f; } //!< ŽQl AShooterPlayerCameraManager::DefaultFOV == 90.0f
	FORCEINLINE virtual float GetSpreadAngle() const { return 0.0f; }
	FORCEINLINE virtual int32 GetSpreadNum() const { return 1; }
	FORCEINLINE virtual float GetRangeDistance() const { return 10000.0f; }
	virtual FVector GetMuzzleLocation() const;
	virtual FVector GetAimDirection() const;
	virtual void GetAim(FVector& Start, FVector& Direction) const;
	virtual bool LineTraceWeapon(const FVector& Start, const FVector& End, FHitResult& HitResult) const;

	FORCEINLINE int32 GetAmmoInClip() const { return AmmoInClip; }
	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE virtual int32 GetAmmoPerClip() const { return 6; }
	FORCEINLINE virtual int32 GetAmmoMax() const { return 3 * GetAmmoPerClip(); }
	virtual int32 GiveAmmo(const int32 Amount);

	virtual void Fire() PURE_VIRTUAL(AShooterWeapon::Fire, );
	
	FORCEINLINE bool IsFiring() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_HandleFiring); }
	virtual void StartFire();
	virtual void StartSimulateFire();
	virtual void EndFire();
	virtual void EndSimulateFire();
	void HandleFiring();
	void RepeatFiring();
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerEndFire();
	virtual bool ServerEndFire_Validate();
	virtual void ServerEndFire_Implementation();
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerHandleFiring();
	virtual bool ServerHandleFiring_Validate();
	virtual void ServerHandleFiring_Implementation();
	UFUNCTION()
	virtual void OnRep_BurstCounter();

	FORCEINLINE bool IsReloading() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_ReloadFinished); }
	virtual void StartReload();
	virtual float StartSimulateReload();
	virtual void EndReload();
	virtual void EndSimulateReload();
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerStartReload();
	virtual bool ServerStartReload_Validate();
	virtual void ServerStartReload_Implementation();
	UFUNCTION()
	virtual void OnRep_Reload();
	virtual void ReloadAmmo();

	virtual void SimulateTargeting(const bool bIsTargeting);

	static void SpawnImpactEffect(UWorld* World, UClass* Class, const FHitResult& HitResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkeletalMesh)
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = Texture)
	UTexture2D* CrosshairTexture;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Reload)
	int32 AmmoInClip;
	UPROPERTY(Transient, Replicated)
	int32 Ammo;
	
	uint8 bWantsToFire : 1;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_BurstCounter)
	int32 BurstCounter;
	FTimerHandle TimerHandle_HandleFiring;

	FTimerHandle TimerHandle_EquipFinished;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Reload)
	uint8 bPendingReload : 1;
	FTimerHandle TimerHandle_ReloadFinished;
	FTimerHandle TimerHandle_ReloadAmmo;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* OwnerFireAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* OwnerReloadAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* OwnerEquipAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimSequence* FireAnimSequence;
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimSequence* ReloadAnimSequence;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue* RaiseSoundCue;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue* LowerSoundCue;
};
