// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ProjectileGrenadeLauncher.h"

#include "ImpactEffectGrenadeLauncher.h"

AProjectileGrenadeLauncher::AProjectileGrenadeLauncher(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (nullptr != StaticMeshComp)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/PrototypeWeap/Prototype_GrenadeLauncher_Ammo.Prototype_GrenadeLauncher_Ammo'"));
		if (SM.Succeeded())
		{
			StaticMeshComp->SetStaticMesh(SM.Object);
		}
		StaticMeshComp->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("ParticleSystem'/Game/PrototypeWeap/FX/P_GrenadeLauncher_Trail.P_GrenadeLauncher_Trail'"));
	if (PS.Succeeded())
	{
		TrailParticle = PS.Object;
	}

	ImpactEffectClass = AImpactEffectGrenadeLauncher::StaticClass();
}
