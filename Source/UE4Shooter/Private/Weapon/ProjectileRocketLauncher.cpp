// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ProjectileRocketLauncher.h"

#include "ImpactEffectRocketLauncher.h"

AProjectileRocketLauncher::AProjectileRocketLauncher(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (nullptr != StaticMeshComp)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/PrototypeWeap/Prototype_RocketLauncher_Ammo.Prototype_RocketLauncher_Ammo'"));
		if (SM.Succeeded())
		{
			StaticMeshComp->SetStaticMesh(SM.Object);
		}
		StaticMeshComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("ParticleSystem'/Game/PrototypeWeap/FX/P_GrenadeLauncher_Trail.P_GrenadeLauncher_Trail'"));
	if (PS.Succeeded())
	{
		TrailParticle = PS.Object;
	}

	ImpactEffectClass = AImpactEffectRocketLauncher::StaticClass();

	static ConstructorHelpers::FObjectFinder<USoundCue> SC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/RocketLauncher/RL_InAirLoop_Cue.RL_InAirLoop_Cue'"));
	if (SC.Succeeded())
	{
		InAirSoundCue = SC.Object;
	}
}

void AProjectileRocketLauncher::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (nullptr != InAirSoundCue)
	{
		InAirSoundComp = UGameplayStatics::SpawnSoundAttached(InAirSoundCue, GetRootComponent());
	}
}

void AProjectileRocketLauncher::SimulateExplode(const FHitResult& HitResult)
{
	Super::SimulateExplode(HitResult);

	if (nullptr != InAirSoundComp)
	{
		InAirSoundComp->FadeOut(1.0f, 0.0f);
	}
}
