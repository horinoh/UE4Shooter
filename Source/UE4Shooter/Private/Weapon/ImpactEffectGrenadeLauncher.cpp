// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ImpactEffectGrenadeLauncher.h"

AImpactEffectGrenadeLauncher::AImpactEffectGrenadeLauncher()
	: Super()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("ParticleSystem'/Game/PrototypeWeap/FX/P_GrenadeLauncher_Explosion_01.P_GrenadeLauncher_Explosion_01'"));
	if (PS.Succeeded())
	{
		ImpactParticle = PS.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/GrenadeLauncher/GL_Explosion_Cue.GL_Explosion_Cue'"));
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
