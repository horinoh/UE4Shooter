// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ShooterProjectile.generated.h"

UCLASS()
class UE4SHOOTER_API AShooterProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AShooterProjectile(const FObjectInitializer& ObjectInitializer);

	//!< AActor
	virtual void PostInitializeComponents() override;
	virtual void PostNetReceiveVelocity(const FVector& NewVelocity) override;

	UFUNCTION()
	void OnProjectileStop(const FHitResult& HitResult);

	UFUNCTION()
	void OnRep_Exploded();
	void SimulateExplode(const FHitResult& HitResult);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StaticMesh)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Exploded)
	uint8 bExploded : 1;
};
