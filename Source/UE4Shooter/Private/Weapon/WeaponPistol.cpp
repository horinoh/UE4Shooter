// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "WeaponPistol.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"

AWeaponPistol::AWeaponPistol(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (nullptr != SkeletalMeshComp)
	{
		//!< メッシュ
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/PrototypeWeap/Prototype_Pistol.Prototype_Pistol'"));
		if (SM.Succeeded())
		{
			SkeletalMeshComp->SetSkeletalMesh(SM.Object);
		}

		//!< アニメーションブループリント
		static ConstructorHelpers::FObjectFinder<UAnimBlueprint> ABP(TEXT("AnimBlueprint'/Game/Shooter/Animation/ABP_Pistol.ABP_Pistol'"));
		if (ABP.Object)
		{
			SkeletalMeshComp->SetAnimInstanceClass(ABP.Object->GetAnimBlueprintGeneratedClass());
		}
	}

	//!< オーナーのアニメーション
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/Fire/AM_Fire_Rifle.AM_Fire_Rifle'")); //!< ライフル流用
	if (FireAM.Succeeded())
	{
		OwnerFireAnimMontage = FireAM.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/Reload/AM_Reload_Pistol.AM_Reload_Pistol'"));
	if (ReloadAM.Succeeded())
	{
		OwnerReloadAnimMontage = ReloadAM.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/Equip/AM_Equip_Pistol.AM_Equip_Pistol'"));
	if (EquipAM.Succeeded())
	{
		OwnerEquipAnimMontage = EquipAM.Object;
	}
	//!< 武器のアニメーション
	static ConstructorHelpers::FObjectFinder<UAnimSequence> FireAS(TEXT("AnimSequence'/Game/PrototypeWeap/Anims/Fire_Pistol_W.Fire_Pistol_W'"));
	if (FireAS.Succeeded())
	{
		FireAnimSequence = FireAS.Object;
	}
}
