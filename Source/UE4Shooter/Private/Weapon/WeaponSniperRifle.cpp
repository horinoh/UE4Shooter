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
		//!< メッシュ
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/PrototypeWeap/Prototype_SniperRifle.Prototype_SniperRifle'"));
		if (SM.Succeeded())
		{
			SkeletalMeshComp->SetSkeletalMesh(SM.Object);
		}

		//!< アニメーションブループリント
		static ConstructorHelpers::FObjectFinder<UAnimBlueprint> ABP(TEXT("AnimBlueprint'/Game/Shooter/Animation/ABP_SniperRifle.ABP_SniperRifle'"));
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
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/Reload/AM_Reload_Rifle.AM_Reload_Rifle'")); //!< ライフル流用
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
	static ConstructorHelpers::FObjectFinder<UAnimSequence> FireAS(TEXT("AnimSequence'/Game/PrototypeWeap/Anims/Fire_SniperRifle_W.Fire_SniperRifle_W'"));
	if (FireAS.Succeeded())
	{
		FireAnimSequence = FireAS.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimSequence> ReloadAS(TEXT("AnimSequence'/Game/PrototypeWeap/Anims/Reload_SniperRifle_Hips_W.Reload_SniperRifle_Hips_W'")); //!< Ironsights, Prone もある
	if (ReloadAS.Succeeded())
	{
		ReloadAnimSequence = ReloadAS.Object;
	}

	ImpactEffectClass = AImpactEffectSniperRifle::StaticClass();
}
