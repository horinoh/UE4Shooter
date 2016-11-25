// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ImpactEffectShotgun.h"

AImpactEffectShotgun::AImpactEffectShotgun()
	: Super()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("ParticleSystem'/Game/PrototypeWeap/FX/P_Impact_01.P_Impact_01'"));
	if (PS.Succeeded())
	{
		ImpactParticle = PS.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/Shotgun/Shotgun_BulletImpactA_Cue.Shotgun_BulletImpactA_Cue'"));
	//static ConstructorHelpers::FObjectFinder<USoundCue> SC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/Shotgun/Shotgun_BulletImpactB_Cue.Shotgun_BulletImpactB_Cue'"));
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
