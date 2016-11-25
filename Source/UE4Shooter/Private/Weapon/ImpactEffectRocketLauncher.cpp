// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ImpactEffectRocketLauncher.h"

AImpactEffectRocketLauncher::AImpactEffectRocketLauncher()
	: Super()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("ParticleSystem'/Game/PrototypeWeap/FX/P_RocketLauncher_Explosion.P_RocketLauncher_Explosion'"));
	if (PS.Succeeded())
	{
		ImpactParticle = PS.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/RocketLauncher/RL_Explosion_Cue.RL_Explosion_Cue'"));
	if (SC.Succeeded())
	{
		ImpactSoundCue = SC.Object;
	}

	//static ConstructorHelpers::FObjectFinder<UMaterialInterface> MI(TEXT(""));
	//if (MI.Succeeded())
	//{
	//	ImpactDecal = MI.Object;
	//}
}
