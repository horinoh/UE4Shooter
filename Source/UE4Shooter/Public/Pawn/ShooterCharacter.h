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

	void StartJump() { Jump(); }
	void EndJump() { StopJumping(); }
	void StartCrouch() { Crouch(); }
	void EndCrouch() { UnCrouch(); }
	void StartSprint();
	void EndSprint();
	void StartTargeting();
	void EndTargeting();
	void StartFire();
	void EndFire();
	void StartReload();

	float GetHealthMax() const;

	void CreateInventory();
	void DestroyInventory();
	UFUNCTION()
	void OnRep_CurrentWeapon(AShooterWeapon* LastWeapon);
	void Equip(AShooterWeapon* NewWeapon);
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerEquip(AShooterWeapon* NewWeapon);
	virtual bool ServerEquip_Validate(AShooterWeapon* NewWeapon);
	virtual void ServerEquip_Implementation(AShooterWeapon* NewWeapon);

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

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TArray<TSubclassOf<AShooterWeapon>> DefaultInventoryClasses;
	UPROPERTY(Transient, Replicated)
	TArray<AShooterWeapon*> Inventory;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentWeapon)
	AShooterWeapon* CurrentWeapon;
};
