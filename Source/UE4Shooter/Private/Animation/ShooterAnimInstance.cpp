// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterAnimInstance.h"

#include "Pawn/ShooterCharacter.h"

UShooterAnimInstance::UShooterAnimInstance()
	: Super()
{
	const auto Locomotion = FName("Locomotion");

	const auto Stand = FName(TEXT("Stand"));
	const auto Crouch = FName(TEXT("Crouch"));
	const auto JumpStart = FName(TEXT("JumpStart"));
	const auto JumpLoop = FName(TEXT("JumpLoop"));
	const auto JumpEnd = FName(TEXT("JumpEnd"));
	const auto Sprint = FName(TEXT("Sprint"));

	//!< �J�ڏ���
	AddNativeTransitionBinding(Locomotion, Stand, Crouch, FCanTakeTransition::CreateUObject(this, &UShooterAnimInstance::ShouldCrouch));
	AddNativeTransitionBinding(Locomotion, Crouch, Stand, FCanTakeTransition::CreateUObject(this, &UShooterAnimInstance::ShouldStand));

	AddNativeTransitionBinding(Locomotion, Stand, JumpStart, FCanTakeTransition::CreateUObject(this, &UShooterAnimInstance::ShouldJumpStart));
	AddNativeTransitionBinding(Locomotion, JumpStart, JumpLoop, FCanTakeTransition::CreateUObject(this, &UShooterAnimInstance::ShouldJumpLoop));
	AddNativeTransitionBinding(Locomotion, JumpLoop, JumpEnd, FCanTakeTransition::CreateUObject(this, &UShooterAnimInstance::ShouldJumpEnd));
	AddNativeTransitionBinding(Locomotion, JumpEnd, Stand, FCanTakeTransition::CreateUObject(this, &UShooterAnimInstance::ShouldStand));

	AddNativeTransitionBinding(Locomotion, Stand, Sprint, FCanTakeTransition::CreateUObject(this, &UShooterAnimInstance::ShouldSprint));
	AddNativeTransitionBinding(Locomotion, Sprint, Stand, FCanTakeTransition::CreateUObject(this, &UShooterAnimInstance::ShouldStand));

	//!< �J�ڃR�[���o�b�N
	AddNativeStateEntryBinding(Locomotion, Stand, FOnGraphStateChanged::CreateUObject(this, &UShooterAnimInstance::StandEntered));
	AddNativeStateEntryBinding(Locomotion, Crouch, FOnGraphStateChanged::CreateUObject(this, &UShooterAnimInstance::CrouchEntered));
	AddNativeStateEntryBinding(Locomotion, JumpStart, FOnGraphStateChanged::CreateUObject(this, &UShooterAnimInstance::JumpStartEntered));
	AddNativeStateEntryBinding(Locomotion, JumpLoop, FOnGraphStateChanged::CreateUObject(this, &UShooterAnimInstance::JumpLoopEntered));
	AddNativeStateEntryBinding(Locomotion, JumpEnd, FOnGraphStateChanged::CreateUObject(this, &UShooterAnimInstance::JumpEndEntered));
	AddNativeStateEntryBinding(Locomotion, Sprint, FOnGraphStateChanged::CreateUObject(this, &UShooterAnimInstance::SprintEntered));
}

void UShooterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const auto PawnOwner = TryGetPawnOwner();
	if (nullptr != PawnOwner)
	{
		//!< �X�s�[�h
		const auto Velocity = PawnOwner->GetVelocity();
		Speed = Velocity.Size2D();

		//!< ����
		Direction = FRotator::NormalizeAxis((Velocity.Rotation() - PawnOwner->GetActorRotation()).Yaw);

		//!< �����A���Ⴊ��
		const auto MovementComp = PawnOwner->GetMovementComponent();
		if (nullptr != MovementComp)
		{
			IsFalling = MovementComp->IsFalling();
			IsCrouching = MovementComp->IsCrouching();
		}

		const auto ShooterCharacter = Cast<AShooterCharacter>(PawnOwner);
		if (nullptr != ShooterCharacter)
		{
			ShooterCharacter->UpdateAimOffset(DeltaSeconds);

			//!< ����
			IsSprinting = ShooterCharacter->IsSprinting();
			//!< �G�C��
			IsTargeting = ShooterCharacter->IsTargeting();

			//!< �G�C���I�t�Z�b�g
			const auto InterpSpeed = 15.0f;
			AimOffsetYaw = FMath::FInterpTo(AimOffsetYaw, ShooterCharacter->GetAimOffsetYaw(), DeltaSeconds, InterpSpeed);
			AimOffsetPitch = FMath::FInterpTo(AimOffsetPitch, ShooterCharacter->GetAimOffsetPitch(), DeltaSeconds, InterpSpeed);
		}
	}
}
