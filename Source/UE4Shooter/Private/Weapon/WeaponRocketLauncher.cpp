// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "WeaponRocketLauncher.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"

#include "ProjectileRocketLauncher.h"

AWeaponRocketLauncher::AWeaponRocketLauncher(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (nullptr != SkeletalMeshComp)
	{
		//!< ���b�V��
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/PrototypeWeap/Prototype_RocketLauncher.Prototype_RocketLauncher'"));
		if (SM.Succeeded())
		{
			SkeletalMeshComp->SetSkeletalMesh(SM.Object);
		}

		//!< �A�j���[�V�����u���[�v�����g
		static ConstructorHelpers::FObjectFinder<UAnimBlueprint> ABP(TEXT("AnimBlueprint'/Game/Shooter/Animation/ABP_RocketLauncher.ABP_RocketLauncher'"));
		if (ABP.Object)
		{
			SkeletalMeshComp->SetAnimInstanceClass(ABP.Object->GetAnimBlueprintGeneratedClass());
		}
	}

	//!< �I�[�i�[�̃A�j���[�V����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/Fire/AM_Fire_Shotgun.AM_Fire_Shotgun'")); //!< �V���b�g�K�����p
	if (FireAM.Succeeded())
	{
		OwnerFireAnimMontage = FireAM.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/Reload/AM_Reload_Shotgun.AM_Reload_Shotgun'")); //!< �V���b�g�K�����p
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
	static ConstructorHelpers::FObjectFinder<UAnimSequence> FireAS(TEXT("AnimSequence'/Game/PrototypeWeap/Anims/Fire_RocketLauncher_W.Fire_RocketLauncher_W'"));
	if (FireAS.Succeeded())
	{
		FireAnimSequence = FireAS.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimSequence> ReloadAS(TEXT("AnimSequence'/Game/PrototypeWeap/Anims/Reload_RocketLauncher_W.Reload_RocketLauncher_W'"));
	if (ReloadAS.Succeeded())
	{
		ReloadAnimSequence = ReloadAS.Object;
	}

	//!< �G�C����
	static ConstructorHelpers::FObjectFinder<USoundCue> RaiseSC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/RocketLauncher/RL_Raise_Cue.RL_Raise_Cue'"));
	if (RaiseSC.Succeeded())
	{
		RaiseSoundCue = RaiseSC.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> LowerSC(TEXT("SoundCue'/Game/PrototypeWeap/Sound/RocketLauncher/RL_Lower_Cue.RL_Lower_Cue'"));
	if (LowerSC.Succeeded())
	{
		LowerSoundCue = LowerSC.Object;
	}

	//!< �v���W�F�N�^�C���N���X
	ProjectileClass = AProjectileRocketLauncher::StaticClass();

	//!< (��[�ɂ��Ă���)�����ڂ����̒e
	StaticMeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMeshComp"));
	if (nullptr != StaticMeshComp)
	{
		StaticMeshComp->bReceivesDecals = false;

		StaticMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
		StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StaticMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);

		StaticMeshComp->SetupAttachment(SkeletalMeshComp, TEXT("Ammo"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/PrototypeWeap/Prototype_RocketLauncher_Ammo.Prototype_RocketLauncher_Ammo'"));
		if (SM.Succeeded())
		{
			StaticMeshComp->SetStaticMesh(SM.Object);
		}
	}
}

void AWeaponRocketLauncher::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetHideAmmo(true);
}

void AWeaponRocketLauncher::UnEquip()
{
	Super::UnEquip();

	SetHideAmmo(true);
}
void AWeaponRocketLauncher::OnEquipFinished()
{
	Super::OnEquipFinished();

	if (0 < AmmoInClip)
	{
		SetHideAmmo(false);
	}
}
void AWeaponRocketLauncher::StartSimulateFire()
{
	Super::StartSimulateFire();

	SetHideAmmo(true);
}
float AWeaponRocketLauncher::StartSimulateReload()
{
	SetHideAmmo(false);

	return Super::StartSimulateReload();
}

void AWeaponRocketLauncher::SetHideAmmo(const bool IsHide)
{
	if (nullptr != StaticMeshComp)
	{
		StaticMeshComp->SetHiddenInGame(IsHide);
	}
}
