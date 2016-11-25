// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "WeaponShotgun.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"

#include "ImpactEffectShotgun.h"

AWeaponShotgun::AWeaponShotgun(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (nullptr != SkeletalMeshComp)
	{
		//!< メッシュ
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/PrototypeWeap/Prototype_Shotgun.Prototype_Shotgun'"));
		if (SM.Succeeded())
		{
			SkeletalMeshComp->SetSkeletalMesh(SM.Object);
		}

		//!< アニメーションブループリント
		static ConstructorHelpers::FObjectFinder<UAnimBlueprint> ABP(TEXT("AnimBlueprint'/Game/Shooter/Animation/ABP_Shotgun.ABP_Shotgun'"));
		if (ABP.Object)
		{
			SkeletalMeshComp->SetAnimInstanceClass(ABP.Object->GetAnimBlueprintGeneratedClass());
		}
	}

	//!< オーナーのアニメーション
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
	static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/Equip/AM_Equip_Rifle.AM_Equip_Rifle'")); //!< ライフル流用
	if (EquipAM.Succeeded())
	{
		OwnerEquipAnimMontage = EquipAM.Object;
	}
	//!< 武器のアニメーション
	static ConstructorHelpers::FObjectFinder<UAnimSequence> FireAS(TEXT("AnimSequence'/Game/PrototypeWeap/Anims/Fire_Shotgun_W.Fire_Shotgun_W'"));
	if (FireAS.Succeeded())
	{
		FireAnimSequence = FireAS.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimSequence> ReloadAS(TEXT("AnimSequence'/Game/PrototypeWeap/Anims/Reload_Shotgun_Hips_W.Reload_Shotgun_Hips_W'")); //!< Ironsights, Prone もある
	if (ReloadAS.Succeeded())
	{
		ReloadAnimSequence = ReloadAS.Object;
	}

	ImpactEffectClass = AImpactEffectShotgun::StaticClass();
}
