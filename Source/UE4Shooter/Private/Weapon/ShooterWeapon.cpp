// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterWeapon.h"

AShooterWeapon::AShooterWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	SetReplicates(true);
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bNetUseOwnerRelevancy = true;

	SkeletalMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("SkeletalMesh"));
	if (nullptr != SkeletalMeshComp)
	{
		SkeletalMeshComp->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
		SkeletalMeshComp->bReceivesDecals = false;

		//!< �V���h�E�}�b�v��������܂Ƃ߂�
		//!< �e�̃��[�g�R���|�[�l���g�ɑ΂��� bLightAttachmentsAsGroup = true �Ƃ��Ă���
		//!< ������� Show - Advanced - Shadow Frustums �Ŋm�F�ł���
		SkeletalMeshComp->bUseAttachParentBound = true;

		//!< �R���W����
		SkeletalMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
		SkeletalMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SkeletalMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		SkeletalMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		SkeletalMeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel_WeaponInstant, ECR_Block);
		SkeletalMeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel_WeaponProjectile, ECR_Block);

		SetRootComponent(SkeletalMeshComp);
	}
}

void AShooterWeapon::Equip(APawn* NewOwner)
{
	//!< Instigator �͂��̃A�N�^�ɂ���Ĉ����N�������_���[�W�̐ӔC��
	Instigator = NewOwner;
	//!< ��Ƀ��v���P�[�V�����ׂ̈ɃI�[�i���w��
	SetOwner(NewOwner);

#if 0
	//!< #TODO �����A�j���[�V������������ OnEquipFinished() ���R�[�������
	const auto Character = Cast<ACharacter>(GetOwner());
	if (nullptr != Character)
	{
		//!< Duration == 0.0f ���ƃ^�C�}���N���A���Ă��܂��̂Œ���
		const auto Duration = FMath::Max(Character->PlayAnimMontage(EquipOwnerAnimMontage), 0.1f);
		GetWorldTimerManager().SetTimer(TimerHandle_EquipFinished, this, &AShooterWeapon::OnEquipFinished, Duration, false);
	}
#else
	//!< ���͒��� OnEquipFinished() ���R�[�����Ă���
	OnEquipFinished();
#endif
}
void AShooterWeapon::UnEquip()
{
	//!< #TODO
	//GetWorldTimerManager().ClearTimer(TimerHandle_EquipFinished);

	if (nullptr != SkeletalMeshComp)
	{
		//!< SetHiddenInGame() �̓��v���P�[�g����Ȃ��̂ŃN���C�A���g�ł��R�[������K�v������
		SkeletalMeshComp->SetHiddenInGame(true);

		//!< DetachFromParent() �̓��v���P�[�g����邪(�x���̂����Ȃ̂�)�N���C�A���g�ł��R�[�����Ă���
		SkeletalMeshComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void AShooterWeapon::OnEquipFinished()
{
	if (nullptr != SkeletalMeshComp)
	{
		//!< SetHiddenInGame() �̓��v���P�[�g����Ȃ��̂ŃN���C�A���g�ł��R�[������K�v������
		SkeletalMeshComp->SetHiddenInGame(false);

		const auto Character = Cast<ACharacter>(GetOwner());
		if (nullptr != Character)
		{
			const auto OwnerSkelMeshComp = Character->GetMesh();
			if (nullptr != OwnerSkelMeshComp)
			{
				//!< AttachTo(), SnapTo() �̓��v���P�[�g����邪(�x���̂����Ȃ̂�)�N���C�A���g�ł��R�[�����Ă���
				SkeletalMeshComp->SnapTo(OwnerSkelMeshComp, TEXT("weapon_r"));
			}
		}
	}
}

