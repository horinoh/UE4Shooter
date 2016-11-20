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

	//!< �N���X�w�A�e�N�X�`��
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairT2D(TEXT("Texture2D'/Game/Crosshair_fps_tutorial/crosshair.crosshair'"));
	if (CrosshairT2D.Succeeded())
	{
		CrosshairTexture = CrosshairT2D.Object;
	}
}

void AShooterWeapon::Equip(APawn* NewOwner)
{
	//!< Instigator �͂��̃A�N�^�ɂ���Ĉ����N�������_���[�W�̐ӔC��
	Instigator = NewOwner;
	//!< ��Ƀ��v���P�[�V�����ׂ̈ɃI�[�i���w��
	SetOwner(NewOwner);

#if 1
	//!< #TODO �����A�j���[�V������������ OnEquipFinished() ���R�[�������
	if (nullptr != OwnerEquipAnimMontage)
	{
		const auto Owner = Cast<ACharacter>(GetOwner());
		if (nullptr != Owner)
		{
			//!< Duration == 0.0f ���ƃ^�C�}���N���A���Ă��܂��̂Œ���
			const auto Duration = FMath::Max(Owner->PlayAnimMontage(OwnerEquipAnimMontage), 0.1f);
			GetWorldTimerManager().SetTimer(TimerHandle_EquipFinished, this, &AShooterWeapon::OnEquipFinished, Duration, false);
		}
	}
#else
	//!< ���͒��� OnEquipFinished() ���R�[�����Ă���
	OnEquipFinished();
#endif
}
void AShooterWeapon::UnEquip()
{
	if (nullptr != SkeletalMeshComp)
	{
		//!< SetHiddenInGame() �̓��v���P�[�g����Ȃ��̂ŃN���C�A���g�ł��R�[������K�v������
		SkeletalMeshComp->SetHiddenInGame(true);

		//!< DetachFromParent() �̓��v���P�[�g����邪(�x���̂����Ȃ̂�)�N���C�A���g�ł��R�[�����Ă���
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

FVector AShooterWeapon::GetMuzzleLocation() const
{
	if (nullptr != SkeletalMeshComp)
	{
		//!< ProrotypeWeapon ���b�V���� MuzzleFlash �\�P�b�g�������Ă���
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

	//!< #TODO �e�������
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
	//!< �I�[�i�[�̃A�j���[�V����
	const auto Character = Cast<ACharacter>(GetOwner());
	if (nullptr != Character)
	{
		if (nullptr != OwnerFireAnimMontage)
		{
			const auto SectionName = FName(TEXT("Default"));
			Character->PlayAnimMontage(OwnerFireAnimMontage, 1.0f, SectionName);
		}
	}

	//!< ����̃A�j���[�V���� (�T�E���h�A�G�t�F�N�g�t��)
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
		//!< �T�[�o���� BurstCounter ���X�V���� �� �N���C�A���g�� OnRep_BurstCounter()
		++BurstCounter;
	} 
	else
	{
		//!< ���[�J���R���g���[���̏ꍇ
		const auto Pawn = Cast<APawn>(GetOwner());
		if (nullptr != Pawn && Pawn->IsLocallyControlled())
		{
			if (bWantsToFire)
			{
				//!< Fire() �͔h���N���X���ɓƎ���������
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

		//!< �T�[�o�֔��C������
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

	//!< �I�[�i�[�̃A�j���[�V����
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

	//!< ����̃A�j���[�V����
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
		//!< �{�^����������Ă���΁A�����[�h�セ�̂܂ܔ��C��
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
