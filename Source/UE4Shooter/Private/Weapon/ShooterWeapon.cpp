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

	SkeletalMeshComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("SkeletalMeshComp"));
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

		//!< 消しておく (装備されたときに出現させる)
		SkeletalMeshComp->SetHiddenInGame(true);

		SetRootComponent(SkeletalMeshComp);
	}

	//!< クロスヘアテクスチャ
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairT2D(TEXT("Texture2D'/Game/Crosshair_fps_tutorial/crosshair.crosshair'"));
	if (CrosshairT2D.Succeeded())
	{
		CrosshairTexture = CrosshairT2D.Object;
	}
}

void AShooterWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AmmoInClip = GetAmmoPerClip();
	Ammo = GetAmmoMax();
}

void AShooterWeapon::Equip(APawn* NewOwner)
{
	//!< Instigator はこのアクタによって引き起こされるダメージの責任者
	Instigator = NewOwner;
	//!< 主にレプリケーションの為にオーナを指定
	SetOwner(NewOwner);

	if (nullptr != OwnerEquipAnimMontage)
	{
		const auto Chara = Cast<ACharacter>(GetOwner());
		if (nullptr != Chara)
		{
			//!< Duration == 0.0f だとタイマをクリアしてしまうので注意
			const auto Duration = FMath::Max(Chara->PlayAnimMontage(OwnerEquipAnimMontage), 0.1f);
			GetWorldTimerManager().SetTimer(TimerHandle_EquipFinished, this, &AShooterWeapon::OnEquipFinished, Duration, false);
		}
	}
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
	GetWorldTimerManager().ClearTimer(TimerHandle_ReloadAmmo);

	EndFire();
	EndReload();
}
void AShooterWeapon::OnEquipFinished()
{
	if (nullptr != SkeletalMeshComp)
	{
		//!< SetHiddenInGame() はレプリケートされないのでクライアントでもコールする必要がある
		SkeletalMeshComp->SetHiddenInGame(false);

		const auto Chara = Cast<ACharacter>(GetOwner());
		if (nullptr != Chara)
		{
			const auto OwnerSkelMeshComp = Chara->GetMesh();
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
		//!< ProrotypeWeapon アセットは MuzzleFlash ソケットを持っている
		return SkeletalMeshComp->GetSocketLocation(TEXT("MuzzleFlash"));
	}
	return FVector::ZeroVector;
}

FVector AShooterWeapon::GetAimDirection() const
{
	if (nullptr != Instigator)
	{
		const auto PC = Cast<APlayerController>(Instigator->GetController());
		if (nullptr != PC)
		{
			FVector Start;
			FRotator Rot;
			PC->GetPlayerViewPoint(Start, Rot);
			return Rot.Vector();
		}
		else
		{
			const auto AC = Cast<AAIController>(Instigator->GetController());
			return nullptr != AC ? AC->GetControlRotation().Vector() : Instigator->GetBaseAimRotation().Vector();
		}
	}
	return FVector::ZeroVector;
}
void AShooterWeapon::GetAim(FVector& Origin, FVector& Direction) const
{
	if (nullptr != Instigator)
	{
		const auto PC = Cast<APlayerController>(Instigator->GetController());
		if (nullptr != PC)
		{
			FRotator Rot;
			PC->GetPlayerViewPoint(Origin, Rot);
			Direction = Rot.Vector();
		}
		else
		{
			Origin = GetMuzzleLocation();
			const auto AC = Cast<AAIController>(Instigator->GetController());
			Direction = nullptr != AC ? AC->GetControlRotation().Vector() : Instigator->GetBaseAimRotation().Vector();
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
		return World->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel_WeaponInstant, Params);
	}
	return false;
}

int32 AShooterWeapon::GiveAmmo(const int32 Amount)
{
	//!< 加算できる最大値
	const auto MaxAdd = FMath::Max(GetAmmoMax() - Ammo, 0);
	//!< 減算できる最小値 (Amountが負の場合)
	const auto MinSub = FMath::Max(-Ammo, FMath::Min(Amount, 0)); 
	const auto AddAmount = FMath::Clamp(Amount, MinSub, MaxAdd);
	Ammo += AddAmount;

	//!< オートリロード
	if (0 < Ammo && 0 >= AmmoInClip)
	{
		StartReload();
	}

	return AddAmount;
}

void AShooterWeapon::StartFire()
{
	if (!bWantsToFire)
	{
		bWantsToFire = true;
	}

	if (0 < AmmoInClip)
	{
		HandleFiring();
	}
	else if(0 < Ammo)
	{
		StartReload();
	}
}
void AShooterWeapon::StartSimulateFire()
{
	//!< オーナーのアニメーション
	const auto Chara = Cast<ACharacter>(GetOwner());
	if (nullptr != Chara)
	{
		if (nullptr != OwnerFireAnimMontage)
		{
			//!< #MY_TODO IsTargeting() の場合に "Ironsights"
			const auto SectionName = FName(TEXT("Default"));
			Chara->PlayAnimMontage(OwnerFireAnimMontage, 1.0f, SectionName);
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

	if (!HasAuthority() && IsLocallyControlled())
	{
		ServerEndFire();
	}
}
void AShooterWeapon::EndSimulateFire()
{
	const auto Chara = Cast<ACharacter>(GetOwner());
	if (nullptr != Chara)
	{
		if (nullptr != OwnerFireAnimMontage)
		{
			Chara->StopAnimMontage(OwnerFireAnimMontage);
		}
	}
}
void AShooterWeapon::HandleFiring()
{
	if (HasAuthority())
	{
		//!< サーバ側は BurstCounter を更新する → クライアントで OnRep_BurstCounter()
		if (0 < AmmoInClip)
		{
			--AmmoInClip;
			--Ammo;
			++BurstCounter;
		}
		else
		{
			BurstCounter = 0;
		}
	} 
	else
	{
		//!< サーバへ発砲させる
		if (bWantsToFire)
		{
			ServerHandleFiring();
		}

		//!< ローカルコントロールなら発砲開始 or 終了の処理をする
		if (IsLocallyControlled())
		{
			if (bWantsToFire)
			{
				if (0 < AmmoInClip)
				{
					//!< Fire() は派生クラス毎に独自実装する
					Fire();

					StartSimulateFire();
					RepeatFiring();
				}
				else if(0 < Ammo)
				{
					StartReload();
				}
			}
			else
			{
				EndSimulateFire();
				GetWorldTimerManager().ClearTimer(TimerHandle_HandleFiring);
			}
		}
	}
}
void AShooterWeapon::RepeatFiring()
{
	if (nullptr != OwnerFireAnimMontage)
	{
		const auto Chara = Cast<ACharacter>(GetOwner());
		if (nullptr != Chara)
		{
			const auto SkelMesh = Chara->GetMesh();
			if (nullptr != SkelMesh)
			{
				const auto AnimInst = SkelMesh->GetAnimInstance();
				if (nullptr != AnimInst)
				{
					const auto SectionName = AnimInst->Montage_GetCurrentSection();
					const auto SectionIndex = OwnerFireAnimMontage->GetSectionIndex(SectionName);
					const auto Duration = FMath::Max(OwnerFireAnimMontage->GetSectionLength(SectionIndex), 0.1f);
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

		//!< 弾の補充
		const auto AmmoDuration = FMath::Max(Duration * 0.8f, 0.1f);
		GetWorldTimerManager().SetTimer(TimerHandle_ReloadAmmo, this, &AShooterWeapon::ReloadAmmo, AmmoDuration, false);
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
		const auto Chara = Cast<ACharacter>(GetOwner());
		if (nullptr != Chara)
		{
			const auto SectionName = FName(TEXT("Default"));

			Chara->PlayAnimMontage(OwnerReloadAnimMontage, 1.0f, SectionName);

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
	else if (bWantsToFire)
	{
		//!< ボタンが押されていれば、そのまま発砲へ
		HandleFiring();
	}
}
void AShooterWeapon::EndSimulateReload()
{
	if (nullptr != OwnerReloadAnimMontage)
	{
		const auto Chara = Cast<ACharacter>(GetOwner());
		if (nullptr != Chara)
		{
			Chara->StopAnimMontage(OwnerReloadAnimMontage);
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
void AShooterWeapon::ReloadAmmo()
{
	const auto ClipDelta = FMath::Min(Ammo, (GetAmmoPerClip() - AmmoInClip));
	if (ClipDelta > 0)
	{
		AmmoInClip += ClipDelta;
	}
}

void AShooterWeapon::SimulateTargeting(const bool bIsTargeting)
{
	const auto Sound = bIsTargeting ? RaiseSoundCue : LowerSoundCue;
	if (nullptr != Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation());
	}
}

void AShooterWeapon::SpawnImpactEffect(UWorld* World, UClass* Class, const FHitResult& HitResult)
{
	if (nullptr != World)
	{
		const auto Location = HitResult.ImpactPoint;
		if (nullptr != Class)
		{
			const auto Rotation = HitResult.ImpactNormal.Rotation();

			auto Effect = World->SpawnActorDeferred<AShooterImpactEffect>(Class, FTransform(Rotation, Location));
			if (nullptr != Effect)
			{
				//!< Surface 属性を見て出し分ける為に FHitResult が必要
				Effect->SetHitResult(HitResult);
				UGameplayStatics::FinishSpawningActor(Effect, FTransform(Rotation, Location));
			}
		}
		else
		{
			DrawDebugSphere(World, Location, 5.0f, 8, FColor::Red, false, 2.0f);
		}
	}
}

void AShooterWeapon::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AShooterWeapon, AmmoInClip, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AShooterWeapon, Ammo, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(AShooterWeapon, BurstCounter, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AShooterWeapon, bPendingReload, COND_SkipOwner);
}
