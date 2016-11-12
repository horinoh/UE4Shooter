// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ShooterWeapon.generated.h"

UCLASS(Abstract)
class UE4SHOOTER_API AShooterWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AShooterWeapon(const FObjectInitializer& ObjectInitializer);

	virtual void Equip(APawn* NewOwner);
	virtual void UnEquip();
	virtual void OnEquipFinished();

	FORCEINLINE UTexture2D* GetCrosshairTexture() const { return CrosshairTexture; }

	virtual void Fire() PURE_VIRTUAL(AShooterWeapon::Fire, );
	
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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkeletalMesh)
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = Texture)
	UTexture2D* CrosshairTexture;

	FTimerHandle TimerHandle_HandleFiring;
	uint8 bWantsToFire : 1;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_BurstCounter)
	int32 BurstCounter;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* OwnerFireAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimSequence* FireAnimSequence;
};
