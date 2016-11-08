// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterCharacter.h"

#include "UnrealNetwork.h"

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

	const auto SkelMesh = GetMesh();
	if (nullptr != SkelMesh)
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
		if (SkeletalMesh.Succeeded())
		{
			SkelMesh->SetSkeletalMesh(SkeletalMesh.Object);
		}
		SkelMesh->SetRelativeLocationAndRotation(FVector::UpVector * -90.0f, FRotator(0.0f, -90.0f, 0.0f));
		
		//!< AlwaysTickPose �͕`�悳��Ă��Ȃ��ƍX�V����Ȃ��̂� DedicateServer �ł͍X�V����Ȃ��AAlwaysTickPoseAndRefreshBones �ɂ���ƕ`�悳��Ă��Ȃ��Ă��X�V�����
		SkelMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;

		SkelMesh->bReceivesDecals = false;

		//!< ���b�V���R���W����
		SkelMesh->SetCollisionObjectType(ECC_Pawn);
		SkelMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SkelMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		SkelMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel_WeaponInstant, ECR_Block);
		SkelMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel_WeaponProjectile, ECR_Block);

		//!< �A�j���[�V����BP
		static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprint(TEXT("AnimBlueprint'/Game/Shooter/Animation/ABP_UE4Mannequin.ABP_UE4Mannequin'"));
		if (AnimBlueprint.Object)
		{
			SkelMesh->SetAnimInstanceClass(AnimBlueprint.Object->GetAnimBlueprintGeneratedClass());
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

void AShooterCharacter::StartSprint()
{

}
void AShooterCharacter::EndSprint()
{

}
void AShooterCharacter::StartTargeting()
{

}
void AShooterCharacter::EndTargeting()
{

}
void AShooterCharacter::StartFire()
{

}
void AShooterCharacter::EndFire()
{

}
void AShooterCharacter::StartReload()
{

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

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AShooterCharacter, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AShooterCharacter, bWantsToSprint, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AShooterCharacter, bIsTargeting, COND_SkipOwner);
}

