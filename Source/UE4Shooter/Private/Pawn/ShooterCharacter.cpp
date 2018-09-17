// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterCharacter.h"

#include "UnrealNetwork.h"
#include "Animation/AnimMontage.h"

#include "ShooterCharaMovementComponent.h"

#include "Player/ShooterPlayerState.h"

#include "Weapon/ShooterDamageType.h"

#include "Weapon/WeaponAssaultRifle.h"
#include "Weapon/WeaponGrenadeLauncher.h"
#include "Weapon/WeaponPistol.h"
#include "Weapon/WeaponRocketLauncher.h"
#include "Weapon/WeaponShotgun.h"
#include "Weapon/WeaponSniperRifle.h"

AShooterCharacter::AShooterCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UShooterCharaMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	const auto CapsuleComp = GetCapsuleComponent();
	if (nullptr != CapsuleComp)
	{
		//!< カメラ
		SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
		if (nullptr != SpringArmComp)
		{
			SpringArmComp->SetupAttachment(CapsuleComp);
			SpringArmComp->bUsePawnControlRotation = true;
			SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 100.0f, 50.0f), FRotator(-20.0f, 0.0f, 0.0f));

			CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
			if (nullptr != CameraComp)
			{
				CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
				CameraComp->bUsePawnControlRotation = false;
			}			
		}
		//bUseControllerRotationYaw = false;

		//!< カプセルコリジョン
		CapsuleComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		CapsuleComp->SetCollisionResponseToChannel(ECC_GameTraceChannel_WeaponInstant, ECR_Ignore);
		CapsuleComp->SetCollisionResponseToChannel(ECC_GameTraceChannel_WeaponProjectile, ECR_Block);

		//!< シャドウマップ視錐台をまとめる
		//!< この設定はルートコンポーネントに対してすること
		//!< 子のコンポーネントに対して bUseAttachParentBound = true しておくこと
		//!< Show - Advanced - Shadow Frustums で視錐台を確認できる
		CapsuleComp->bLightAttachmentsAsGroup = true;
	}

	const auto SkelMeshComp = GetMesh();
	if (nullptr != SkelMeshComp)
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
		if (SkeletalMesh.Succeeded())
		{
			SkelMeshComp->SetSkeletalMesh(SkeletalMesh.Object);
		}
		SkelMeshComp->SetRelativeLocationAndRotation(FVector::UpVector * -90.0f, FRotator(0.0f, -90.0f, 0.0f));
		
		//!< AlwaysTickPose は描画されていないと更新されないので DedicateServer では更新されない、AlwaysTickPoseAndRefreshBones にすると描画されていなくても更新される
		SkelMeshComp->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;

		SkelMeshComp->bReceivesDecals = false;

		//!< メッシュコリジョン
		SkelMeshComp->SetCollisionObjectType(ECC_Pawn);
		SkelMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SkelMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		SkelMeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel_WeaponInstant, ECR_Block);
		SkelMeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel_WeaponProjectile, ECR_Block);

		//!< アニメーションBP
#if 1
		static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Shooter/Animation/ABP_UE4Mannequin"));
		//static ConstructorHelpers::FObjectFinder<UClass> AnimBPClass(TEXT("Class'/Game/Shooter/Animation/ABP_UE4Mannequin.ABP_UE4Mannequin_C'"));
		if (AnimBPClass.Succeeded())
		{
			SkelMeshComp->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			SkelMeshComp->SetAnimInstanceClass(AnimBPClass.Class);
			//SkelMeshComp->SetAnimInstanceClass(AnimBPClass.Object);
		}
#else
		//!< ↓このようにはできない
		//!< BP はエディタ外では必要とされないので BP ではなく直接 Class へアクセスしないとパッケージ化した時に読み込めない
		//!< https://answers.unrealengine.com/questions/127212/bdontloadblueprintoutsideeditortrue-by-default.html
		//!< https://answers.unrealengine.com/questions/130256/whats-the-correct-way-to-load-blueprints-for-packa.html
		static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBPClass(TEXT("AnimBlueprint'/Game/Shooter/Animation/ABP_UE4Mannequin.ABP_UE4Mannequin'"));
		if (AnimBPClass.Succeeded())
		{
			SkelMeshComp->SetAnimInstanceClass(AnimBPClass.Object->GetAnimBlueprintGeneratedClass());
		}
#endif
	}

	const auto MovementComp = GetCharacterMovement();
	if (nullptr != MovementComp)
	{
		//!< カプセルを短くしても最下点を維持する (カプセルの中心を下げる)
		//!< (普通に短くするとカプセルの最上下点が中心に寄ることになり浮くことになる)
		MovementComp->bCrouchMaintainsBaseLocation = true;
		MovementComp->bMaintainHorizontalGroundVelocity = false;

		//!< デフォルト値が false なので true にしないとしゃがめない
		MovementComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	}

	//!< アニメーション
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathAM(TEXT("AnimMontage'/Game/Shooter/Animation/Montage/AM_Death.AM_Death'"));
	if (DeathAM.Succeeded())
	{
		DeathAnimMontage = DeathAM.Object;
	}

	//!< インベントリ
	DefaultInventoryClasses.AddUnique(AWeaponAssaultRifle::StaticClass());
	//DefaultInventoryClasses.AddUnique(AWeaponGrenadeLauncher::StaticClass());
	//DefaultInventoryClasses.AddUnique(AWeaponPistol::StaticClass());
	DefaultInventoryClasses.AddUnique(AWeaponRocketLauncher::StaticClass());
	DefaultInventoryClasses.AddUnique(AWeaponShotgun::StaticClass());
	DefaultInventoryClasses.AddUnique(AWeaponSniperRifle::StaticClass());
}

void AShooterCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}
void AShooterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const auto SkelMesh = GetMesh();
	if (nullptr != SkelMesh)
	{
		for (auto i = 0; i < SkelMesh->GetNumMaterials(); ++i)
		{
			MaterialInstanceDynamics.Add(SkelMesh->CreateAndSetMaterialInstanceDynamic(i));
		}
	}

	CreateInventory();
}
float AShooterCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const auto ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (0.0f != ActualDamage)
	{
		Health -= ActualDamage;

		if (FPointDamageEvent::ClassID == DamageEvent.GetTypeID())
		{
			const auto PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		}
		else if (FRadialDamageEvent::ClassID == DamageEvent.GetTypeID())
		{
			const auto RadialDamageEvent = static_cast<const FRadialDamageEvent*>(&DamageEvent);
		}

		{
			const auto DamageType = (nullptr != DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>());
			EventInstigator = GetDamageInstigator(EventInstigator, *DamageType);
		}
		const auto PawnInstigator = nullptr != EventInstigator ? EventInstigator->GetPawn() : nullptr;

		const auto World = GetWorld();
		if (nullptr != World)
		{
			const auto Time = World->GetTimeSeconds();
			if (TakeHitTime == Time)
			{
				TakeHitInfo.Damage += Damage;
				UE_LOG(LogShooter, Warning, TEXT("Damage = %f (+%f)\n"), TakeHitInfo.Damage, Damage);
			}
			else
			{
				TakeHitInfo.Damage = Damage;
				UE_LOG(LogShooter, Warning, TEXT("Damage = %f\n"), TakeHitInfo.Damage);
			}
			TakeHitTime = Time;
		}
		TakeHitInfo.bKilled = 0.0f >= Health;
		TakeHitInfo.PawnInstigator = PawnInstigator;
		TakeHitInfo.DamageCauser = DamageCauser;
		TakeHitInfo.SetDamageEvent(DamageEvent);

		//!< フォースフィードバック
		const auto PC = Cast<APlayerController>(GetController());
		if (nullptr != PC && nullptr != DamageEvent.DamageTypeClass)
		{
			const auto DamageType = Cast<UShooterDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject());
			if(nullptr != DamageType)
			{
				const auto FFE = TakeHitInfo.bKilled ? DamageType->KilledForceFeedbackEffect : DamageType->HitForceFeedbackEffect;
				if (nullptr != FFE)
				{
					UE_LOG(LogShooter, Log, TEXT("ForceFeedbackEffect\n"));
					PC->ClientPlayForceFeedback(FFE, false, false, TEXT("ForceFeedbackEffect"));
				}
			}
		}

		if (TakeHitInfo.bKilled)
		{
			Die();
		}
		else {
			Hit();
		}
	}

	return ActualDamage;
}
void AShooterCharacter::PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	const auto World = GetWorld();
	DOREPLIFETIME_ACTIVE_OVERRIDE(AShooterCharacter, TakeHitInfo, nullptr != World && World->GetTimeSeconds() < TakeHitTime + 0.5f);
}

void AShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (nullptr != PlayerInputComponent)
	{
		PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
		PlayerInputComponent->BindAxis("Turn", this, &AShooterCharacter::AddControllerYawInput);
		PlayerInputComponent->BindAxis("LookUp", this, &AShooterCharacter::AddControllerPitchInput);

		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooterCharacter::StartJump);
		PlayerInputComponent->BindAction("Jump", IE_Released, this, &AShooterCharacter::EndJump);
		PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AShooterCharacter::StartCrouch);
		PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AShooterCharacter::EndCrouch);
		PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AShooterCharacter::StartSprint);
		PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AShooterCharacter::EndSprint);
		PlayerInputComponent->BindAction("Targeting", IE_Pressed, this, &AShooterCharacter::StartTargeting);
		PlayerInputComponent->BindAction("Targeting", IE_Released, this, &AShooterCharacter::EndTargeting);
		PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShooterCharacter::StartFire);
		PlayerInputComponent->BindAction("Fire", IE_Released, this, &AShooterCharacter::EndFire);
		PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AShooterCharacter::StartReload);

		PlayerInputComponent->BindAction("Equip0", IE_Pressed, this, &AShooterCharacter::Equip0);
		PlayerInputComponent->BindAction("Equip1", IE_Pressed, this, &AShooterCharacter::Equip1);
		PlayerInputComponent->BindAction("Equip2", IE_Pressed, this, &AShooterCharacter::Equip2);
		PlayerInputComponent->BindAction("Equip3", IE_Pressed, this, &AShooterCharacter::Equip3);
	}
}

void AShooterCharacter::SetPlayerDefaults()
{
	Super::SetPlayerDefaults();

	Health = GetHealthMax();
}
void AShooterCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UpdateTeamColors();
}
void AShooterCharacter::Destroyed()
{
	Super::Destroyed();

	DestroyInventory();
}
void AShooterCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UpdateTeamColors();
}

void AShooterCharacter::Jump()
{
	if (CanJump())
	{
		Super::Jump();
	}
}
void AShooterCharacter::Crouch(bool bClientSimulation)
{
	if (CanCrouch())
	{
		Super::Crouch(bClientSimulation);
	}
}

bool AShooterCharacter::CanCrouch()
{
	if (Super::CanCrouch())
	{
		const auto PC = Cast<APlayerController>(GetController());
		if (nullptr != PC && !PC->bCinematicMode)
		{
			return !IsTargeting() && !IsJumpProvidingForce();
		}
	}
	return false;
}
void AShooterCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	//UpdatePawnMeshes();
	//SetCurrentWeapon(CurrentWeapon);
	UpdateTeamColors();
}

bool AShooterCharacter::CanJump() const
{
	const auto PC = Cast<APlayerController>(GetController());
	if (nullptr != PC && !PC->bCinematicMode)
	{
		return !IsKilled() && !IsTargeting();
	}
	return false;
}

bool AShooterCharacter::CanMove() const
{
	const auto PC = Cast<APlayerController>(GetController());
	if (nullptr != PC && !PC->bCinematicMode)
	{
		return !IsKilled() && !IsTargeting();
	}
	return false;
}
void AShooterCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		if (CanMove())
		{
			AddMovementInput(GetActorForwardVector(), Value);
		}
	}
}
void AShooterCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		if (CanMove())
		{
			AddMovementInput(GetActorRightVector(), Value);
		}
	}
}

void AShooterCharacter::Hit()
{
	SimulateHit();
}
void AShooterCharacter::SimulateHit()
{
	if (0.0f < TakeHitInfo.Damage)
	{
		ApplyDamageMomentum(TakeHitInfo.Damage, TakeHitInfo.GetDamageEvent(), TakeHitInfo.PawnInstigator.Get(), TakeHitInfo.DamageCauser.Get());
	}
}
void AShooterCharacter::Die()
{
	Health = FMath::Max(0.0f, Health);

	//!< レプリケート更新
	NetUpdateFrequency = GetDefault<AShooterCharacter>()->NetUpdateFrequency;
	const auto MovementComp = GetCharacterMovement();
	if (nullptr != MovementComp)
	{
		MovementComp->ForceReplicationUpdate();
	}
	
	SimulateDie();
}
void AShooterCharacter::SimulateDie()
{
	TearOff();
	SetReplicateMovement(false);

	//if (GetNetMode() != NM_DedicatedServer && DeathSound && Mesh1P && Mesh1P->IsVisible())
	//{
	//	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	//}

	DestroyInventory();

	//!< カプセルコリジョンを無効化
	const auto CapsuleComp = GetCapsuleComponent();
	if (nullptr != CapsuleComp)
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	StopAnimMontage();

	//!< 死亡アニメーション再生
	const auto DeathAnimDuration = [&]()
	{
		if (nullptr != DeathAnimMontage)
		{
			const auto SectionName = FName(TEXT("Default"));
			PlayAnimMontage(DeathAnimMontage, 1.0f, SectionName);
			const auto SectionIndex = DeathAnimMontage->GetSectionIndex(SectionName);
			//!< ブレンドで元の位置に戻されないように、アニメーションが終わる少し前にラグドールを起動する
			return FMath::Max(DeathAnimMontage->GetSectionLength(SectionIndex) - 0.5f, 0.1f);
		}
		return 0.1f;
	}();

	//!< 死亡アニメーション後ラグドールへ
	FTimerHandle TimerHandle_RagdollPhysics;
	GetWorldTimerManager().SetTimer(TimerHandle_RagdollPhysics, this, &AShooterCharacter::SetRagdollPhysics, DeathAnimDuration, false);
}
void AShooterCharacter::SetRagdollPhysics()
{
	const auto MovementComp = GetCharacterMovement();
	if (nullptr != MovementComp)
	{
		MovementComp->StopMovementImmediately();
		MovementComp->DisableMovement();
		MovementComp->SetComponentTickEnabled(false);
	}

	const auto SkelMesh = GetMesh();
	if (nullptr != SkelMesh)
	{
		SkelMesh->SetCollisionProfileName(FName(TEXT("Ragdoll")));
	}
	SetActorEnableCollision(true);
	if (!IsPendingKill() && nullptr != SkelMesh && nullptr != SkelMesh->GetPhysicsAsset())
	{
		SkelMesh->SetAllBodiesSimulatePhysics(true);
		SkelMesh->SetSimulatePhysics(true);
		SkelMesh->WakeAllRigidBodies();
		SkelMesh->bBlendPhysics = true;

		SetLifeSpan(5.0f);
	}
	else
	{
		TurnOff();
		SetActorHiddenInGame(true);

		SetLifeSpan(1.0f);
	}

	//!< コントローラからデタッチ
	FTimerHandle TimerHandle_Detach;
	GetWorldTimerManager().SetTimer(TimerHandle_Detach, this, &AShooterCharacter::DetachFromControllerPendingDestroy, GetLifeSpan(), false);
}

void AShooterCharacter::OnRep_TakeHitInfo()
{
	if (TakeHitInfo.bKilled)
	{
		SimulateDie();
	}
	else
	{
		SimulateHit();
	}
}

bool AShooterCharacter::CanSprint() const
{
	const auto PC = Cast<APlayerController>(GetController());
	if (nullptr != PC && !PC->bCinematicMode)
	{
		return !IsPendingKill() && (nullptr == CurrentWeapon || (!CurrentWeapon->IsEquipping() && !CurrentWeapon->IsReloading() && !CurrentWeapon->IsFiring()));
	}
	return false;
}
bool AShooterCharacter::IsSprinting() const
{
	//!< 前方速度が無いと「走り」にはしない
	const auto Velocity = GetVelocity();
	return bWantsToSprint && !Velocity.IsZero() && (Velocity.GetSafeNormal2D() | GetActorForwardVector()) > FMath::Cos(FMath::DegreesToRadians(30.f));
}
void AShooterCharacter::SetSprint(bool bNewSprint)
{
	if (CanSprint())
	{
		bWantsToSprint = bNewSprint;

		if (!HasAuthority())
		{
			ServerSetSprint(bNewSprint);
		}

		//if (IsLocallyControlled())
		//{
		//	SimulateSprint();
		//}
	}
}
bool AShooterCharacter::ServerSetSprint_Validate(bool bNewSprint)
{
	return true;
}
void AShooterCharacter::ServerSetSprint_Implementation(bool bNewSprint)
{
	SetSprint(bNewSprint);
}

bool AShooterCharacter::CanTargeting() const
{
	const auto PC = Cast<APlayerController>(GetController());
	if (nullptr != PC && !PC->bCinematicMode)
	{
		if (!IsSprinting())
		{
			return !IsKilled() && (nullptr != CurrentWeapon && !CurrentWeapon->IsEquipping() && !CurrentWeapon->IsReloading());
		}
	}
	return false;
}
bool AShooterCharacter::IsTargeting() const
{
	return bIsTargeting;
}
void AShooterCharacter::SetTargeting(bool bNewTargeting)
{
	if (CanTargeting())
	{
		bIsTargeting = bNewTargeting;

		if (!HasAuthority())
		{
			ServerSetTargeting(bNewTargeting);
		}

		if (IsLocallyControlled())
		{
			SimulateTargeting();
		}
	}
}
bool AShooterCharacter::ServerSetTargeting_Validate(bool bNewTargeting)
{
	return true;
}
void AShooterCharacter::ServerSetTargeting_Implementation(bool bNewTargeting)
{
	SetTargeting(bNewTargeting);
}
void AShooterCharacter::SimulateTargeting()
{
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->SimulateTargeting(bIsTargeting);
	}
}

void AShooterCharacter::UpdateAimOffset(float DeltaSeconds)
{
	if (IsLocallyControlled())
	{
		if(!IsKilled())
		{
			const auto CurrentRot = FRotator(AimOffsetPitch, AimOffsetYaw, 0.0f);
			const auto TargetRot = GetControlRotation() - GetActorRotation();
			const auto InterpSpeed = 15.0f;
			const auto NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, InterpSpeed);

			if (!bUseControllerRotationYaw)
			{
				AimOffsetYaw = FMath::Clamp(FRotator::NormalizeAxis(NewRot.Yaw), -90.0f, 90.0f);
			}
			AimOffsetPitch = FMath::Clamp(FRotator::NormalizeAxis(NewRot.Pitch), -90.0f, 90.0f);
			
			ServerSetAimOffset(AimOffsetYaw, AimOffsetPitch);
		}
	}
}
bool AShooterCharacter::ServerSetAimOffset_Validate(float Yaw, float Pitch)
{
	return true;
}
void AShooterCharacter::ServerSetAimOffset_Implementation(float Yaw, float Pitch)
{
	AimOffsetYaw = Yaw;
	AimOffsetPitch = Pitch;
}

float AShooterCharacter::GetHealthMax() const
{
	const auto Class = GetClass();
	if (nullptr != Class)
	{
		const auto DefaultObject = Class->GetDefaultObject<AShooterCharacter>();
		if (nullptr != DefaultObject)
		{
			return DefaultObject->Health;
		}
	}
	return 1.0f;
}

void AShooterCharacter::CreateInventory()
{
	if (HasAuthority())
	{
		const auto World = GetWorld();
		if (nullptr != World)
		{
			for (const auto i : DefaultInventoryClasses)
			{
				if (nullptr != i)
				{
					FActorSpawnParameters Params;
					Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
					const auto Weapon = World->SpawnActor<AShooterWeapon>(i, Params);
					if (nullptr != Weapon)
					{
						//!< Instigator はこのアクタによって引き起こされるダメージの責任者
						Weapon->Instigator = this;
						//!< 主にレプリケーションの為にオーナを指定
						Weapon->SetOwner(this);

						Inventory.AddUnique(Weapon);
					}
				}
			}
		}
		if (0 < Inventory.Num())
		{
			Equip(Inventory[0]);
		}
	}
}
void AShooterCharacter::DestroyInventory()
{
	if (HasAuthority())
	{
		//!< ケツから消すこと
		for (auto i = Inventory.Num() - 1; i >= 0; --i)
		{
			const auto Weapon = Inventory[i];
			if (nullptr != Weapon)
			{
				Inventory.RemoveSingle(Weapon);
				Weapon->Destroy();
			}
		}
	}

}
void AShooterCharacter::OnRep_CurrentWeapon(AShooterWeapon* LastWeapon)
{
	if (nullptr != LastWeapon)
	{
		LastWeapon->UnEquip();
	}
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->Equip(this);
	}
}

bool AShooterCharacter::CanFire() const
{
	const auto PC = Cast<APlayerController>(GetController());
	if (nullptr != PC && !PC->bCinematicMode)
	{
		return !IsKilled() && !IsSprinting() && nullptr != CurrentWeapon && !CurrentWeapon->IsEquipping() && !CurrentWeapon->IsReloading() && !CurrentWeapon->IsFiring();
	}
	return false;
}
void AShooterCharacter::StartFire()
{
	if (CanFire())
	{
		const auto Weapon = Cast<AShooterWeapon>(CurrentWeapon);
		if (nullptr != Weapon)
		{
			Weapon->StartFire();
		}
	}
}
void AShooterCharacter::EndFire()
{
	const auto Weapon = Cast<AShooterWeapon>(CurrentWeapon);
	if (nullptr != Weapon)
	{
		Weapon->EndFire();
	}
}

bool AShooterCharacter::CanReload() const
{
	const auto PC = Cast<APlayerController>(GetController());
	if (nullptr != PC && !PC->bCinematicMode)
	{
		return !IsKilled() && nullptr != CurrentWeapon && !CurrentWeapon->IsEquipping() && !CurrentWeapon->IsReloading();
	}
	return false;
}
void AShooterCharacter::StartReload()
{
	const auto PC = Cast<APlayerController>(GetController());
	if (nullptr != PC && !PC->bCinematicMode)
	{
		if (CanReload())
		{
			const auto Weapon = Cast<AShooterWeapon>(CurrentWeapon);
			if (nullptr != Weapon)
			{
				//!< #MY_TODO
				//if (0 < Weapon->GetAmmo() && Weapon->GetAmmoInClip() < Weapon->GetAmmoPerClip())
				{
					Weapon->StartReload();
				}
			}
		}
	}
}

bool AShooterCharacter::CanEquip() const
{
	const auto PC = Cast<APlayerController>(GetController());
	if (nullptr != PC && !PC->bCinematicMode)
	{
		return !IsKilled() && (nullptr == CurrentWeapon || (!CurrentWeapon->IsEquipping() && !CurrentWeapon->IsReloading()));
	}
	return false;
}
void AShooterCharacter::Equip(AShooterWeapon* NewWeapon)
{
	if (nullptr != NewWeapon)
	{
		if (HasAuthority())
		{
			auto LastWeapon = GetWeapon();
			CurrentWeapon = NewWeapon;

			OnRep_CurrentWeapon(LastWeapon);
		}
		else
		{
			ServerEquip(NewWeapon);
		}

		//if (IsLocallyControlled())
		//{
		//	SimulateEquip();
		//}
	}
}
void AShooterCharacter::Equip(const int32 Index)
{
	if (CanEquip())
	{
		const auto Num = Inventory.Num();
		if (1 < Num && Index < Num)
		{
			auto NextWeapon = Inventory[Index];
			if (CurrentWeapon != NextWeapon)
			{
				Equip(NextWeapon);
			}
		}
	}
}
void AShooterCharacter::EquipPrev()
{
	const auto Num = Inventory.Num();
	const auto Index = Inventory.IndexOfByKey(CurrentWeapon);
	Equip(INDEX_NONE == Index ? 0 : ((Index - 1 + Num) % Num));
}
void AShooterCharacter::EquipNext()
{
	const auto Num = Inventory.Num();
	const auto Index = Inventory.IndexOfByKey(CurrentWeapon);
	Equip(INDEX_NONE == Index ? 0 : ((Index + 1 + Num) % Num));
}

bool AShooterCharacter::ServerEquip_Validate(AShooterWeapon* NewWeapon)
{
	return true;
}
void AShooterCharacter::ServerEquip_Implementation(AShooterWeapon* NewWeapon)
{
	Equip(NewWeapon);
}

void AShooterCharacter::UpdateTeamColors()
{
	const auto PS = Cast<AShooterPlayerState>(PlayerState);
	if (nullptr != PS)
	{
		const TArray<FLinearColor> Colors = {
			FLinearColor::Red,
			FLinearColor::Blue,
			FLinearColor::Green,
			FLinearColor::Yellow,
		};
		const auto TeamNumber = PS->GetTeamNumber();
		//!< UE4_Mannequin のマテリアルは "BodyColor" がベクタパラメータとして外にに出ている
		for (const auto i : MaterialInstanceDynamics)
		{
			i->SetVectorParameterValue(TEXT("BodyColor"), Colors[TeamNumber]);
		}
	}
}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AShooterCharacter, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AShooterCharacter, TakeHitInfo, COND_Custom);
	//DOREPLIFETIME(AShooterCharacter, TakeHitInfo);

	DOREPLIFETIME_CONDITION(AShooterCharacter, bWantsToSprint, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AShooterCharacter, bIsTargeting, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AShooterCharacter, AimOffsetYaw, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AShooterCharacter, AimOffsetPitch, COND_SkipOwner);

	DOREPLIFETIME_CONDITION(AShooterCharacter, Inventory, COND_OwnerOnly);
	DOREPLIFETIME(AShooterCharacter, CurrentWeapon);
}

