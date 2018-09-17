// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "WeaponShotgun.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"

#include "ImpactEffectShotgun.h"

AWeaponShotgun::AWeaponShotgun()
{
	if (nullptr != SkeletalMeshComp)
	{
		//!< ���b�V��
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/PrototypeWeap/Prototype_Shotgun.Prototype_Shotgun'"));
		if (SM.Succeeded())
		{
			SkeletalMeshComp->SetSkeletalMesh(SM.Object);
		}

		//!< �A�j���[�V�����u���[�v�����g
		static ConstructorHelpers::FObjectFinder<UClass> AnimBP(TEXT("Class'/Game/Shooter/Animation/ABP_Shotgun.ABP_Shotgun_C'"));
		if (AnimBP.Succeeded())
		{
			SkeletalMeshComp->SetAnimInstanceClass(AnimBP.Object);
		}
	}

	//!< �I�[�i�[�̃A�j���[�V����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/Fire/AM_Fire_Shotgun.AM_Fire_Shotgun'"));
	if (FireAM.Succeeded())
	{
		OwnerFireAnimMontage = FireAM.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/Reload/AM_Reload_Shotgun.AM_Reload_Shotgun'"));
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
	static ConstructorHelpers::FObjectFinder<UAnimSequence> FireAS(TEXT("AnimSequence'/Game/PrototypeWeap/Anims/Fire_Shotgun_W.Fire_Shotgun_W'"));
	if (FireAS.Succeeded())
	{
		FireAnimSequence = FireAS.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimSequence> ReloadAS(TEXT("AnimSequence'/Game/PrototypeWeap/Anims/Reload_Shotgun_Hips_W.Reload_Shotgun_Hips_W'")); //!< Ironsights, Prone ������
	if (ReloadAS.Succeeded())
	{
		ReloadAnimSequence = ReloadAS.Object;
	}

	//!< �G�C����
	static ConstructorHelpers::FObjectFinder<USoundCue> RaiseSC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/Shotgun/Shotgun_Raise_Cue.Shotgun_Raise_Cue'"));
	if (RaiseSC.Succeeded())
	{
		RaiseSoundCue = RaiseSC.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> LowerSC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/Shotgun/Shotgun_Lower_Cue.Shotgun_Lower_Cue'"));
	if (LowerSC.Succeeded())
	{
		LowerSoundCue = LowerSC.Object;
	}

	//!< �C���p�N�g�G�t�F�N�g
	ImpactEffectClass = AImpactEffectShotgun::StaticClass();
}
