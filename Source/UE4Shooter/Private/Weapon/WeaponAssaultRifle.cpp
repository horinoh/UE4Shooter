// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "WeaponAssaultRifle.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"

#include "ImpactEffectAssaultRifle.h"

AWeaponAssaultRifle::AWeaponAssaultRifle()
{
	if (nullptr != SkeletalMeshComp)
	{
		//!< メッシュ
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/PrototypeWeap/Prototype_AssaultRifle.Prototype_AssaultRifle'"));
		if (SM.Succeeded())
		{
			SkeletalMeshComp->SetSkeletalMesh(SM.Object);
		}

		//!< アニメーションブループリント
		static ConstructorHelpers::FObjectFinder<UClass> AnimBP(TEXT("Class'/Game/Shooter/Animation/ABP_AssaultRifle.ABP_AssaultRifle_C'"));
		if (AnimBP.Succeeded())
		{
			SkeletalMeshComp->SetAnimInstanceClass(AnimBP.Object);
		}
	}

	//!< オーナーのアニメーション
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/Fire/AM_Fire_Rifle.AM_Fire_Rifle'"));
	if (FireAM.Succeeded())
	{
		OwnerFireAnimMontage = FireAM.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/Reload/AM_Reload_Rifle.AM_Reload_Rifle'"));
	if (ReloadAM.Succeeded())
	{
		OwnerReloadAnimMontage = ReloadAM.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/Equip/AM_Equip_Rifle.AM_Equip_Rifle'"));
	if (EquipAM.Succeeded())
	{
		OwnerEquipAnimMontage = EquipAM.Object;
	}
	//!< 武器のアニメーション
	static ConstructorHelpers::FObjectFinder<UAnimSequence> FireAS(TEXT("AnimSequence'/Game/PrototypeWeap/Anims/Fire_Rifle_W.Fire_Rifle_W'"));
	if (FireAS.Succeeded())
	{
		FireAnimSequence = FireAS.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimSequence> ReloadAS(TEXT("AnimSequence'/Game/PrototypeWeap/Anims/Reload_Rifle_Hips_W.Reload_Rifle_Hips_W'")); //!< Ironsights, Prone もある
	if (ReloadAS.Succeeded())
	{
		ReloadAnimSequence = ReloadAS.Object;
	}

	//!< エイム音
	static ConstructorHelpers::FObjectFinder<USoundCue> RaiseSC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/Rifle/Rifle_Raise_Cue.Rifle_Raise_Cue'"));
	if (RaiseSC.Succeeded())
	{
		RaiseSoundCue = RaiseSC.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> LowerSC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/Rifle/Rifle_Lower_Cue.Rifle_Lower_Cue'"));
	if (LowerSC.Succeeded())
	{
		LowerSoundCue = LowerSC.Object;
	}

	//!< インパクトエフェクト
	ImpactEffectClass = AImpactEffectAssaultRifle::StaticClass();
}
