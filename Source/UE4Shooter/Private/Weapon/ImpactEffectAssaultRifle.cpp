// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ImpactEffectAssaultRifle.h"

AImpactEffectAssaultRifle::AImpactEffectAssaultRifle()
	: Super()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("ParticleSystem'/Game/PrototypeWeap/FX/P_Impact_01.P_Impact_01'"));
	if (PS.Succeeded())
	{
		ImpactParticle = PS.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/Rifle/Rifle_BulletImpact_Cue.Rifle_BulletImpact_Cue'"));
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
