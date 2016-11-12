// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterCharacter.h"

#include "UnrealNetwork.h"

//#include "Weapon/ShooterWeapon.h"
#include "Weapon/WeaponAssaultRifle.h"
#include "Weapon/WeaponGrenadeLauncher.h"
#include "Weapon/WeaponPistol.h"
#include "Weapon/WeaponRocketLauncher.h"
#include "Weapon/WeaponShotgun.h"
#include "Weapon/WeaponSniperRifle.h"

AShooterCharacter::AShooterCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	const auto CapsuleComp = GetCapsuleComponent();
	if (nullptr != CapsuleComp)
	{
		//!< �J����
		SpringArmComp = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("SpringArm"));
		if (nullptr != SpringArmComp)
		{
			SpringArmComp->SetupAttachment(CapsuleComp);
			SpringArmComp->bUsePawnControlRotation = true;
			SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 100.0f, 50.0f), FRotator(-20.0f, 0.0f, 0.0f));

			CameraComp = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
			if (nullptr != CameraComp)
			{
				CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
				CameraComp->bUsePawnControlRotation = false;
			}			
		}
		//bUseControllerRotationYaw = false;

		//!< �J�v�Z���R���W����
		CapsuleComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		CapsuleComp->SetCollisionResponseToChannel(ECC_GameTraceChannel_WeaponInstant, ECR_Ignore);
		CapsuleComp->SetCollisionResponseToChannel(ECC_GameTraceChannel_WeaponProjectile, ECR_Block);

		//!< �V���h�E�}�b�v��������܂Ƃ߂�
		//!< ���̐ݒ�̓��[�g�R���|�[�l���g�ɑ΂��Ă��邱��
		//!< �q�̃R���|�[�l���g�ɑ΂��� bUseAttachParentBound = true ���Ă�������
		//!< Show - Advanced - Shadow Frustums �Ŏ�������m�F�ł���
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
		
		//!< AlwaysTickPose �͕`�悳��Ă��Ȃ��ƍX�V����Ȃ��̂� DedicateServer �ł͍X�V����Ȃ��AAlwaysTickPoseAndRefreshBones �ɂ���ƕ`�悳��Ă��Ȃ��Ă��X�V�����
		SkelMeshComp->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;

		SkelMeshComp->bReceivesDecals = false;

		//!< ���b�V���R���W����
		SkelMeshComp->SetCollisionObjectType(ECC_Pawn);
		SkelMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SkelMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		SkelMeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel_WeaponInstant, ECR_Block);
		SkelMeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel_WeaponProjectile, ECR_Block);

		//!< �A�j���[�V����BP
		static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprint(TEXT("AnimBlueprint'/Game/Shooter/Animation/ABP_UE4Mannequin.ABP_UE4Mannequin'"));
		if (AnimBlueprint.Object)
		{
			SkelMeshComp->SetAnimInstanceClass(AnimBlueprint.Object->GetAnimBlueprintGeneratedClass());
		}
	}

	const auto MovementComp = GetCharacterMovement();
	if (nullptr != MovementComp)
	{
		//!< �J�v�Z����Z�����Ă��ŉ��_���ێ����� (�J�v�Z���̒��S��������)
		//!< (���ʂɒZ������ƃJ�v�Z���̍ŏ㉺�_�����S�Ɋ�邱�ƂɂȂ蕂�����ƂɂȂ�)
		MovementComp->bCrouchMaintainsBaseLocation = true;
		MovementComp->bMaintainHorizontalGroundVelocity = false;

		//!< �f�t�H���g�l�� false �Ȃ̂� true �ɂ��Ȃ��Ƃ��Ⴊ�߂Ȃ�
		MovementComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	}

	//!< �C���x���g��
	DefaultInventoryClasses.AddUnique(AWeaponAssaultRifle::StaticClass());
	DefaultInventoryClasses.AddUnique(AWeaponGrenadeLauncher::StaticClass());
	DefaultInventoryClasses.AddUnique(AWeaponPistol::StaticClass());
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
}
void AShooterCharacter::Destroyed()
{
	Super::Destroyed();

	DestroyInventory();
}

void AShooterCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}
void AShooterCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AShooterCharacter::StartFire()
{
	//if (CanFire())
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
void AShooterCharacter::StartReload()
{

}

bool AShooterCharacter::IsSprinting() const
{
	//!< �O�����x�������Ɓu����v�ɂ͂��Ȃ�
	const auto Velocity = GetVelocity();
	return bWantsToSprint && !Velocity.IsZero() && (Velocity.GetSafeNormal2D() | GetActorForwardVector()) > FMath::Cos(FMath::DegreesToRadians(30.f));
}
void AShooterCharacter::SetSprint(bool bNewSprint)
{
	bWantsToSprint = bNewSprint;

	if (false == HasAuthority())
	{
		ServerSetSprint(bNewSprint);
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

bool AShooterCharacter::IsTargeting() const
{
	return bIsTargeting;
}
void AShooterCharacter::SetTargeting(bool bNewTargeting)
{
	bIsTargeting = bNewTargeting;

	if (false == HasAuthority())
	{
		ServerSetTargeting(bNewTargeting);
	}

	//!< #TODO �G�C���� (���v���P�[�g���Ă��Ȃ��A���������ł�����)
	//const auto Weapon = Cast<AShooterWeapon_Shooting>(CurrentWeapon);
	//if (nullptr != Weapon)
	//{
	//	Weapon->SimulateTargeting(bIsTargeting);
	//}
}
bool AShooterCharacter::ServerSetTargeting_Validate(bool bNewTargeting)
{
	return true;
}
void AShooterCharacter::ServerSetTargeting_Implementation(bool bNewTargeting)
{
	SetTargeting(bNewTargeting);
}

void AShooterCharacter::UpdateAimOffset(float DeltaSeconds)
{
	//if (CanTargeting())
	{
		const auto CurrentRot = FRotator(AimOffsetPitch, AimOffsetYaw, 0.0f);
		const auto TargetRot = GetControlRotation() - GetActorRotation();
		const auto InterpSpeed = 15.0f;
		const auto NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, InterpSpeed);

		if (false == bUseControllerRotationYaw)
		{
			AimOffsetYaw = FMath::Clamp(FRotator::NormalizeAxis(NewRot.Yaw), -90.0f, 90.0f);
		}
		AimOffsetPitch = FMath::Clamp(FRotator::NormalizeAxis(NewRot.Pitch), -90.0f, 90.0f);
	}
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
						//!< Instigator �͂��̃A�N�^�ɂ���Ĉ����N�������_���[�W�̐ӔC��
						Weapon->Instigator = this;
						//!< ��Ƀ��v���P�[�V�����ׂ̈ɃI�[�i���w��
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
		//!< �P�c�����������
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
	if (nullptr != GetWeapon())
	{
		GetWeapon()->Equip(this);
	}
}

void AShooterCharacter::Equip(AShooterWeapon* NewWeapon)
{
	if (nullptr != NewWeapon)
	{
		if (HasAuthority())
		{
			auto LastWeapon = GetWeapon();
			//!< CurrentWeapon �̓��v���P�[�g�����̂ŁA���ʓI�ɃN���C�A���g�ł� OnRep_CurrentWeapon() �������
			CurrentWeapon = NewWeapon;
			OnRep_CurrentWeapon(LastWeapon);
		}
		else
		{
			ServerEquip(NewWeapon);
		}
	}
}
void AShooterCharacter::Equip(const int32 Index)
{
	//if (CanEquip())
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

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AShooterCharacter, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AShooterCharacter, bWantsToSprint, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AShooterCharacter, bIsTargeting, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AShooterCharacter, AimOffsetYaw, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AShooterCharacter, AimOffsetPitch, COND_SkipOwner);

	DOREPLIFETIME(AShooterCharacter, CurrentWeapon);
}

