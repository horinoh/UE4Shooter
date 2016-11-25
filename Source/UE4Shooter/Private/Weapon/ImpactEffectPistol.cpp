// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ImpactEffectPistol.h"

AImpactEffectPistol::AImpactEffectPistol()
	: Super()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("ParticleSystem'/Game/PrototypeWeap/FX/P_Impact_01.P_Impact_01'"));
	if (PS.Succeeded())
	{
		ImpactParticle = PS.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/Pistol/Pistol_BulletImpact_Cue.Pistol_BulletImpact_Cue'"));
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
