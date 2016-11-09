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

		//!< シャドウマップ視錐台をまとめる
		//!< 親のルートコンポーネントに対して bLightAttachmentsAsGroup = true としておく
		//!< 視錐台は Show - Advanced - Shadow Frustums で確認できる
		SkeletalMeshComp->bUseAttachParentBound = true;

		//!< コリジョン
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
	//!< Instigator はこのアクタによって引き起こされるダメージの責任者
	Instigator = NewOwner;
	//!< 主にレプリケーションの為にオーナを指定
	SetOwner(NewOwner);

#if 0
	//!< #TODO 装備アニメーション完了時に OnEquipFinished() がコールされる
	const auto Character = Cast<ACharacter>(GetOwner());
	if (nullptr != Character)
	{
		//!< Duration == 0.0f だとタイマをクリアしてしまうので注意
		const auto Duration = FMath::Max(Character->PlayAnimMontage(EquipOwnerAnimMontage), 0.1f);
		GetWorldTimerManager().SetTimer(TimerHandle_EquipFinished, this, &AShooterWeapon::OnEquipFinished, Duration, false);
	}
#else
	//!< 今は直接 OnEquipFinished() をコールしている
	OnEquipFinished();
#endif
}
void AShooterWeapon::UnEquip()
{
	//!< #TODO
	//GetWorldTimerManager().ClearTimer(TimerHandle_EquipFinished);

	if (nullptr != SkeletalMeshComp)
	{
		//!< SetHiddenInGame() はレプリケートされないのでクライアントでもコールする必要がある
		SkeletalMeshComp->SetHiddenInGame(true);

		//!< DetachFromParent() はレプリケートされるが(遅れるのが嫌なので)クライアントでもコールしている
		SkeletalMeshComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void AShooterWeapon::OnEquipFinished()
{
	if (nullptr != SkeletalMeshComp)
	{
		//!< SetHiddenInGame() はレプリケートされないのでクライアントでもコールする必要がある
		SkeletalMeshComp->SetHiddenInGame(false);

		const auto Character = Cast<ACharacter>(GetOwner());
		if (nullptr != Character)
		{
			const auto OwnerSkelMeshComp = Character->GetMesh();
			if (nullptr != OwnerSkelMeshComp)
			{
				//!< AttachTo(), SnapTo() はレプリケートされるが(遅れるのが嫌なので)クライアントでもコールしている
				SkeletalMeshComp->SnapTo(OwnerSkelMeshComp, TEXT("weapon_r"));
			}
		}
	}
}

