// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AShooterWeapon;

USTRUCT()
struct FTakeHitInfo
{
	GENERATED_BODY()

public:
	FTakeHitInfo() { GetDamageEvent().DamageTypeClass = PointDamageEvent.DamageTypeClass = RadialDamageEvent.DamageTypeClass = UDamageType::StaticClass(); }

	FDamageEvent& GetDamageEvent() /*const*/
	{
		switch (DamageEventClassID)
		{
		default: return DamageEvent;
		case FPointDamageEvent::ClassID: return PointDamageEvent;
		case FRadialDamageEvent::ClassID: return RadialDamageEvent;
		}
	}
	void SetDamageEvent(const FDamageEvent& val)
	{
		DamageEventClassID = val.GetTypeID();
		switch (DamageEventClassID)
		{
		default: DamageEvent = val; break;
		case FPointDamageEvent::ClassID: PointDamageEvent = static_cast<const FPointDamageEvent&>(val); break;
		case FRadialDamageEvent::ClassID: RadialDamageEvent = static_cast<const FRadialDamageEvent&>(val); break;
		}
	}

	UPROPERTY()
	float Damage;
	UPROPERTY()
	TWeakObjectPtr<class APawn> PawnInstigator;
	UPROPERTY()
	TWeakObjectPtr<class AActor> DamageCauser;
	UPROPERTY()
	uint8 bKilled : 1;

private:
	UPROPERTY()
	int32 DamageEventClassID;
	UPROPERTY()
	UClass* DamageTypeClass;
	UPROPERTY()
	FDamageEvent DamageEvent;
	UPROPERTY()
	FPointDamageEvent PointDamageEvent;
	UPROPERTY()
	FRadialDamageEvent RadialDamageEvent;
};

UCLASS()
class UE4SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AShooterCharacter(const FObjectInitializer& ObjectInitializer);
	
	//!< AActor
	virtual void Tick( float DeltaSeconds ) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker) override;

	//!< APawn
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SetPlayerDefaults() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Destroyed() override;
	virtual void OnRep_PlayerState() override;

	//!< ACharacter
	virtual void Jump() override;
	virtual void Crouch(bool bClientSimulation = false) override;
	virtual bool CanCrouch() override;
	virtual void PawnClientRestart() override;

	bool CanJump() const;

	bool CanMove() const;
	void MoveForward(float Value);
	void MoveRight(float Value);

	FORCEINLINE void StartJump() { Jump(); }
	FORCEINLINE void EndJump() { StopJumping(); }
	FORCEINLINE void StartCrouch() { Crouch(); }
	FORCEINLINE void EndCrouch() { UnCrouch(); }
	FORCEINLINE void StartSprint() { SetSprint(true); }
	FORCEINLINE void EndSprint() { SetSprint(false); }
	FORCEINLINE void StartTargeting() { SetTargeting(true); }
	FORCEINLINE void EndTargeting() { SetTargeting(false); }
	FORCEINLINE void Equip0() { Equip(0); }
	FORCEINLINE void Equip1() { Equip(1); }
	FORCEINLINE void Equip2() { Equip(2); }
	FORCEINLINE void Equip3() { Equip(3); }

	void Hit();
	void SimulateHit();
	void Die();
	void SimulateDie();
	void SetRagdollPhysics();
	UFUNCTION()
	void OnRep_TakeHitInfo();
	FORCEINLINE bool IsKilled() const { return TakeHitInfo.bKilled; }

	bool CanSprint() const;
	bool IsSprinting() const;
	void SetSprint(bool bNewSprint);
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetSprint(bool bNewSprint);
	virtual bool ServerSetSprint_Validate(bool bNewSprint);
	virtual void ServerSetSprint_Implementation(bool bNewSprint);
	//void SimulateSprint() {}

	bool CanTargeting() const;
	bool IsTargeting() const;
	void SetTargeting(bool bNewTargeting);
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetTargeting(bool bNewTargeting);
	virtual bool ServerSetTargeting_Validate(bool bNewTargeting);
	virtual void ServerSetTargeting_Implementation(bool bNewTargeting);
	void SimulateTargeting();

	FORCEINLINE float GetSpeedScale() const { return (IsSprinting() ? 1.5f : 1.0f) * (IsTargeting() ? 0.75f : 1.0f); }

	void UpdateAimOffset(float DeltaSeconds);
	FORCEINLINE float GetAimOffsetYaw() const { return AimOffsetYaw; }
	FORCEINLINE float GetAimOffsetPitch() const { return AimOffsetPitch; }
	UFUNCTION(Unreliable, Server, WithValidation)
	void ServerSetAimOffset(float Yaw, float Pitch);
	virtual bool ServerSetAimOffset_Validate(float Yaw, float Pitch);
	virtual void ServerSetAimOffset_Implementation(float Yaw, float Pitch);

	float GetHealthMax() const;

	void CreateInventory();
	void DestroyInventory();
	UFUNCTION()
	void OnRep_CurrentWeapon(AShooterWeapon* LastWeapon);
	FORCEINLINE AShooterWeapon* GetWeapon() const { return CurrentWeapon; }

	bool CanFire() const;
	void StartFire();
	void EndFire();

	bool CanReload() const;
	void StartReload();

	bool CanEquip() const;
	void Equip(AShooterWeapon* NewWeapon);
	void Equip(const int32 Index);
	void EquipPrev();
	void EquipNext();
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerEquip(AShooterWeapon* NewWeapon);
	virtual bool ServerEquip_Validate(AShooterWeapon* NewWeapon);
	virtual void ServerEquip_Implementation(AShooterWeapon* NewWeapon);
	//void SimulateEquip() {}

	void UpdateTeamColors();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraComp;

	UPROPERTY(Transient)
	TArray<UMaterialInstanceDynamic*> MaterialInstanceDynamics;

	UPROPERTY(VisibleAnywhere, Replicated)
	float Health = 100.0f;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_TakeHitInfo)
	struct FTakeHitInfo TakeHitInfo;
	float TakeHitTime;

	UPROPERTY(Transient, Replicated)
	uint8 bWantsToSprint : 1;
	UPROPERTY(Transient, Replicated)
	uint8 bIsTargeting : 1;
	
	UPROPERTY(Transient, Replicated)
	float AimOffsetYaw;
	UPROPERTY(Transient, Replicated)
	float AimOffsetPitch;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TArray<TSubclassOf<AShooterWeapon>> DefaultInventoryClasses;
	UPROPERTY(Transient, Replicated)
	TArray<AShooterWeapon*> Inventory;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentWeapon)
	AShooterWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* DeathAnimMontage;
};
