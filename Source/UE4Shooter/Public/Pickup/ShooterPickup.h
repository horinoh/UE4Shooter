// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ShooterPickup.generated.h"

UCLASS()
class UE4SHOOTER_API AShooterPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AShooterPickup(const FObjectInitializer& ObjectInitializer);

	//!< AActor
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION()
	void OnRep_IsActive();
	
	virtual void Respawn();
	virtual void PickedUp();

	virtual bool GiveTo(AActor* OtherActor) PURE_VIRTUAL(AShooterPickup::GiveTo, return false;);
	bool GiveAmmoTo(class AShooterWeapon* Weapon);

	virtual void SimulatePickedUp();
	virtual void SimulateRespawn();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StaticMesh)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue* PickupSoundCue;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsActive)
	uint8 bIsActive : 1;
	
	FTimerHandle TimerHandle_Respawn;
};
