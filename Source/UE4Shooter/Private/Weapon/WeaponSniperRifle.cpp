// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "WeaponSniperRifle.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"

#include "ImpactEffectSniperRifle.h"

AWeaponSniperRifle::AWeaponSniperRifle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (nullptr != SkeletalMeshComp)
	{
		//!< ���b�V��
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/PrototypeWeap/Prototype_SniperRifle.Prototype_SniperRifle'"));
		if (SM.Succeeded())
		{
			SkeletalMeshComp->SetSkeletalMesh(SM.Object);
		}

		//!< �A�j���[�V�����u���[�v�����g
		static ConstructorHelpers::FObjectFinder<UClass> AnimBP(TEXT("Class'/Game/Shooter/Animation/ABP_SniperRifle.ABP_SniperRifle_C'"));
		if (AnimBP.Succeeded())
		{
			SkeletalMeshComp->SetAnimInstanceClass(AnimBP.Object);
		}
	}

	//!< �I�[�i�[�̃A�j���[�V����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/Fire/AM_Fire_Rifle.AM_Fire_Rifle'")); //!< ���C�t�����p
	if (FireAM.Succeeded())
	{
		OwnerFireAnimMontage = FireAM.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/Reload/AM_Reload_Rifle.AM_Reload_Rifle'")); //!< ���C�t�����p
	if (ReloadAM.Succeeded())
	{
		OwnerReloadAnimMontage = ReloadAM.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/Equip/AM_Equip_Rifle.AM_Equip_Rifle'")); //!< ���C�t�����p
	if (EquipAM.Succeeded())
	{
		OwnerEquipAnimMontage = EquipAM.Object;
	}
	//!< ����̃A�j���[�V����
	static ConstructorHelpers::FObjectFinder<UAnimSequence> FireAS(TEXT("AnimSequence'/Game/PrototypeWeap/Anims/Fire_SniperRifle_W.Fire_SniperRifle_W'"));
	if (FireAS.Succeeded())
	{
		FireAnimSequence = FireAS.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimSequence> ReloadAS(TEXT("AnimSequence'/Game/PrototypeWeap/Anims/Reload_SniperRifle_Hips_W.Reload_SniperRifle_Hips_W'")); //!< Ironsights, Prone ������
	if (ReloadAS.Succeeded())
	{
		ReloadAnimSequence = ReloadAS.Object;
	}

	ImpactEffectClass = AImpactEffectSniperRifle::StaticClass();

	//!< �G�C����
	static ConstructorHelpers::FObjectFinder<USoundCue> RaiseSC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/SniperRifle/SR_Raise_Cue.SR_Raise_Cue'"));
	if (RaiseSC.Succeeded())
	{
		RaiseSoundCue = RaiseSC.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> LowerSC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/SniperRifle/SR_Lower_Cue.SR_Lower_Cue'"));
	if (LowerSC.Succeeded())
	{
		LowerSoundCue = LowerSC.Object;
	}

	//!< �Y�[����
	static ConstructorHelpers::FObjectFinder<USoundCue> ZoomInSC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/SniperRifle/SR_ZoomIn_Cue.SR_ZoomIn_Cue'"));
	if (ZoomInSC.Succeeded())
	{
		ZoomInSoundCue = ZoomInSC.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> ZoomOutSC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/SniperRifle/SR_ZoomOut_Cue.SR_ZoomOut_Cue'"));
	if (ZoomOutSC.Succeeded())
	{
		ZoomOutSoundCue = ZoomOutSC.Object;
	}
}
