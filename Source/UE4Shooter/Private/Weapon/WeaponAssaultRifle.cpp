// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "WeaponAssaultRifle.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"

AWeaponAssaultRifle::AWeaponAssaultRifle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (nullptr != SkeletalMeshComp)
	{
		//!< ���b�V��
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/PrototypeWeap/Prototype_AssaultRifle.Prototype_AssaultRifle'"));
		if (SM.Succeeded())
		{
			SkeletalMeshComp->SetSkeletalMesh(SM.Object);
		}

		//!< �A�j���[�V�����u���[�v�����g
		static ConstructorHelpers::FObjectFinder<UAnimBlueprint> ABP(TEXT("AnimBlueprint'/Game/Shooter/Animation/ABP_AssaultRifle.ABP_AssaultRifle'"));
		if (ABP.Object)
		{
			SkeletalMeshComp->SetAnimInstanceClass(ABP.Object->GetAnimBlueprintGeneratedClass());
		}
	}

	//!< �I�[�i�[�̃A�j���[�V����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/AM_Fire_AssaultRifle.AM_Fire_AssaultRifle'"));
	if (FireAM.Succeeded())
	{
		OwnerFireAnimMontage = FireAM.Object;
	}
	//!< ����̃A�j���[�V����
	static ConstructorHelpers::FObjectFinder<UAnimSequence> FireAS(TEXT("AnimSequence'/Game/PrototypeWeap/Anims/Fire_Rifle_W.Fire_Rifle_W'"));
	if (FireAS.Succeeded())
	{
		FireAnimSequence = FireAS.Object;
	}
}
