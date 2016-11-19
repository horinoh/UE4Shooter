// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AShooterWeapon;

UCLASS()
class UE4SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AShooterCharacter(const FObjectInitializer& ObjectInitializer);
	
	//!< AActor
	virtual void Tick( float DeltaSeconds ) override;
	virtual void PostInitializeComponents() override;

	//!< APawn
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SetPlayerDefaults() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Destroyed() override;

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
	void StartFire();
	void EndFire();
	void StartReload();

	bool IsSprinting() const;
	void SetSprint(bool bNewSprint);
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetSprint(bool bNewSprint);
	virtual bool ServerSetSprint_Validate(bool bNewSprint);
	virtual void ServerSetSprint_Implementation(bool bNewSprint);

	bool IsTargeting() const;
	void SetTargeting(bool bNewTargeting);
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetTargeting(bool bNewTargeting);
	virtual bool ServerSetTargeting_Validate(bool bNewTargeting);
	virtual void ServerSetTargeting_Implementation(bool bNewTargeting);

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
	void Equip(AShooterWeapon* NewWeapon);
	void Equip(const int32 Index);
	void EquipPrev();
	void EquipNext();
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerEquip(AShooterWeapon* NewWeapon);
	virtual bool ServerEquip_Validate(AShooterWeapon* NewWeapon);
	virtual void ServerEquip_Implementation(AShooterWeapon* NewWeapon);

	FORCEINLINE AShooterWeapon* GetWeapon() const { return CurrentWeapon; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraComp;

	UPROPERTY(Transient)
	TArray<UMaterialInstanceDynamic*> MaterialInstanceDynamics;

	UPROPERTY(VisibleAnywhere, Replicated)
	float Health;
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
};
