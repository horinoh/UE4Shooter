// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

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
};
