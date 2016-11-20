// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterWeapon.h"

#include "UnrealNetwork.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"

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

	//!< クロスヘアテクスチャ
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairT2D(TEXT("Texture2D'/Game/Crosshair_fps_tutorial/crosshair.crosshair'"));
	if (CrosshairT2D.Succeeded())
	{
		CrosshairTexture = CrosshairT2D.Object;
	}
}

void AShooterWeapon::Equip(APawn* NewOwner)
{
	//!< Instigator はこのアクタによって引き起こされるダメージの責任者
	Instigator = NewOwner;
	//!< 主にレプリケーションの為にオーナを指定
	SetOwner(NewOwner);

#if 1
	//!< #TODO 装備アニメーション完了時に OnEquipFinished() がコールされる
	if (nullptr != OwnerEquipAnimMontage)
	{
		const auto Owner = Cast<ACharacter>(GetOwner());
		if (nullptr != Owner)
		{
			//!< Duration == 0.0f だとタイマをクリアしてしまうので注意
			const auto Duration = FMath::Max(Owner->PlayAnimMontage(OwnerEquipAnimMontage), 0.1f);
			GetWorldTimerManager().SetTimer(TimerHandle_EquipFinished, this, &AShooterWeapon::OnEquipFinished, Duration, false);
		}
	}
#else
	//!< 今は直接 OnEquipFinished() をコールしている
	OnEquipFinished();
#endif
}
void AShooterWeapon::UnEquip()
{
	if (nullptr != SkeletalMeshComp)
	{
		//!< SetHiddenInGame() はレプリケートされないのでクライアントでもコールする必要がある
		SkeletalMeshComp->SetHiddenInGame(true);

		//!< DetachFromParent() はレプリケートされるが(遅れるのが嫌なので)クライアントでもコールしている
		SkeletalMeshComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}

	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFiring);
	GetWorldTimerManager().ClearTimer(TimerHandle_EquipFinished);
	GetWorldTimerManager().ClearTimer(TimerHandle_ReloadFinished);

	EndFire();
	EndReload();
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

FVector AShooterWeapon::GetMuzzleLocation() const
{
	if (nullptr != SkeletalMeshComp)
	{
		//!< ProrotypeWeapon メッシュは MuzzleFlash ソケットを持っている
		return SkeletalMeshComp->GetSocketLocation(TEXT("MuzzleFlash"));
	}
	return FVector::ZeroVector;
}

void AShooterWeapon::GetAim(FVector& Start, FVector& Direction) const
{
	if (nullptr != Instigator)
	{
		const auto PC = Cast<APlayerController>(Instigator->GetController());
		if (nullptr != PC)
		{
			FRotator Rot;
			PC->GetPlayerViewPoint(Start, Rot);
			Direction = Rot.Vector();
		}
		else
		{
			Start = GetMuzzleLocation();
			const auto AC = Cast<AAIController>(Instigator->GetController());
			if (nullptr != AC)
			{
				Direction = AC->GetControlRotation().Vector();
			}
			else
			{
				Direction = Instigator->GetBaseAimRotation().Vector();
			}
		}
	}
}

bool AShooterWeapon::LineTraceWeapon(const FVector& Start, const FVector& End, FHitResult& HitResult) const
{
	const auto World = GetWorld();
	if (nullptr != World)
	{
		FCollisionQueryParams Params(TEXT("FireTag"), true, Instigator);
		Params.bTraceAsyncScene = true;
		Params.bReturnPhysicalMaterial = true;
		if (World->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel_WeaponInstant, Params))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}
void AShooterWeapon::StartFire()
{
	if (!bWantsToFire)
	{
		bWantsToFire = true;
	}

	//!< #TODO 弾があれば
	if (true)
	{
		HandleFiring();
	}
	else
	{
		//Reload();
	}
}
void AShooterWeapon::StartSimulateFire()
{
	//!< オーナーのアニメーション
	const auto Character = Cast<ACharacter>(GetOwner());
	if (nullptr != Character)
	{
		if (nullptr != OwnerFireAnimMontage)
		{
			const auto SectionName = FName(TEXT("Default"));
			Character->PlayAnimMontage(OwnerFireAnimMontage, 1.0f, SectionName);
		}
	}

	//!< 武器のアニメーション (サウンド、エフェクト付き)
	if (nullptr != SkeletalMeshComp)
	{
		const auto AnimInst = SkeletalMeshComp->GetAnimInstance();
		if (nullptr != AnimInst)
		{
			if (nullptr != FireAnimSequence)
			{
				AnimInst->PlaySlotAnimationAsDynamicMontage(FireAnimSequence, TEXT("DefaultSlot"));
			}
		}
	}
}
void AShooterWeapon::EndFire()
{
	if (bWantsToFire)
	{
		bWantsToFire = false;
	}

	BurstCounter = 0;

	if (false == HasAuthority())
	{
		const auto Pawn = Cast<APawn>(GetOwner());
		if (nullptr != Pawn && Pawn->IsLocallyControlled())
		{
			ServerEndFire();
		}
	}
}
void AShooterWeapon::EndSimulateFire()
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (nullptr != Character)
	{
		if (nullptr != OwnerFireAnimMontage)
		{
			Character->StopAnimMontage(OwnerFireAnimMontage);
		}
	}
}
void AShooterWeapon::HandleFiring()
{
	if (HasAuthority())
	{
		//!< サーバ側は BurstCounter を更新する → クライアントで OnRep_BurstCounter()
		++BurstCounter;
	} 
	else
	{
		//!< ローカルコントロールの場合
		const auto Pawn = Cast<APawn>(GetOwner());
		if (nullptr != Pawn && Pawn->IsLocallyControlled())
		{
			if (bWantsToFire)
			{
				//!< Fire() は派生クラス毎に独自実装する
				Fire();

				StartSimulateFire();
				RepeatFiring();
			}
			else
			{
				EndSimulateFire();
				GetWorldTimerManager().ClearTimer(TimerHandle_HandleFiring);
			}
		}

		//!< サーバへ発砲させる
		if (bWantsToFire)
		{	
			ServerHandleFiring();
		}
	}
}
void AShooterWeapon::RepeatFiring()
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (nullptr != Character)
	{
		const auto SkelMesh = Character->GetMesh();
		if (nullptr != SkelMesh)
		{
			const auto AnimInst = SkelMesh->GetAnimInstance();
			if (nullptr != AnimInst)
			{
				const auto SectionName = AnimInst->Montage_GetCurrentSection();
				if (nullptr != OwnerFireAnimMontage)
				{
					const auto SectionIndex = OwnerFireAnimMontage->GetSectionIndex(SectionName);
					const auto Duration = OwnerFireAnimMontage->GetSectionLength(SectionIndex);
					GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AShooterWeapon::HandleFiring, Duration, false);
				}
			}
		}
	}
}
bool AShooterWeapon::ServerEndFire_Validate()
{
	return true;
}
void AShooterWeapon::ServerEndFire_Implementation()
{
	EndFire();
}
bool AShooterWeapon::ServerHandleFiring_Validate()
{
	return true;
}
void AShooterWeapon::ServerHandleFiring_Implementation()
{
	HandleFiring();
}
void AShooterWeapon::OnRep_BurstCounter()
{
	if (0 < BurstCounter)
	{
		StartSimulateFire();
	}
	else
	{
		EndSimulateFire();
	}
}

void AShooterWeapon::StartReload()
{
	const auto Duration = StartSimulateReload();

	if (HasAuthority())
	{
		bPendingReload = true;
	}
	else
	{
		ServerStartReload();
	}
}
float AShooterWeapon::StartSimulateReload()
{
	auto Duration = 0.0f;

	//!< オーナーのアニメーション
	if (nullptr != OwnerReloadAnimMontage)
	{
		const auto Owner = Cast<ACharacter>(GetOwner());
		if (nullptr != Owner)
		{
			const auto SectionName = FName(TEXT("Default"));

			Owner->PlayAnimMontage(OwnerReloadAnimMontage, 1.0f, SectionName);

			const auto SectionIndex = OwnerReloadAnimMontage->GetSectionIndex(SectionName);
			Duration = FMath::Max(OwnerReloadAnimMontage->GetSectionLength(SectionIndex), 0.1f);
		}
		GetWorldTimerManager().SetTimer(TimerHandle_ReloadFinished, this, &AShooterWeapon::EndReload, Duration, false);
	}

	//!< 武器のアニメーション
	if (nullptr != SkeletalMeshComp)
	{
		const auto AnimInst = SkeletalMeshComp->GetAnimInstance();
		if (nullptr != AnimInst)
		{
			const auto SlotName = TEXT("DefaultSlot");
			AnimInst->PlaySlotAnimationAsDynamicMontage(ReloadAnimSequence, SlotName);
		}
	}

	return Duration;
}
void AShooterWeapon::EndReload()
{
	if (HasAuthority())
	{
		bPendingReload = false;
	}
	else
	{
		//!< ボタンが押されていれば、リロード後そのまま発砲へ
		if (bWantsToFire)
		{
			if (nullptr != OwnerFireAnimMontage)
			{
				const auto Owner = Cast<ACharacter>(GetOwner());
				if (nullptr != Owner)
				{
					const auto SkelMesh = Owner->GetMesh();
					if (nullptr != SkelMesh)
					{
						const auto AnimInst = SkelMesh->GetAnimInstance();
						if (nullptr != AnimInst)
						{
							const auto SectionName = AnimInst->Montage_GetCurrentSection();
							const auto SectionIndex = OwnerFireAnimMontage->GetSectionIndex(SectionName);
							const auto Duration = OwnerFireAnimMontage->GetSectionLength(SectionIndex);
							GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AShooterWeapon::HandleFiring, Duration, false);
						}
					}
				}
			}
		}
	}
}
void AShooterWeapon::EndSimulateReload()
{
	if (nullptr != OwnerReloadAnimMontage)
	{
		const auto Character = Cast<ACharacter>(GetOwner());
		if (nullptr != Character)
		{
			Character->StopAnimMontage(OwnerReloadAnimMontage);
		}
	}
}
bool AShooterWeapon::ServerStartReload_Validate()
{
	return true;
}
void AShooterWeapon::ServerStartReload_Implementation()
{
	StartReload();
}
void AShooterWeapon::OnRep_Reload()
{
	if (bPendingReload)
	{
		StartSimulateReload();
	}
	else
	{
		EndSimulateReload();
	}
}

void AShooterWeapon::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AShooterWeapon, BurstCounter, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AShooterWeapon, bPendingReload, COND_SkipOwner);
}
