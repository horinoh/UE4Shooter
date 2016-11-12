// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNode_StateMachine.h"

#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE4SHOOTER_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UShooterAnimInstance();

	//!< UAnimInstance
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	FORCEINLINE bool ShouldCrouch() const { return IsCrouching && !IsTargeting; }
	UFUNCTION()
	FORCEINLINE bool ShouldStand() const { return !IsCrouching && !IsSprinting && !IsTargeting && !IsFalling; }
	UFUNCTION()
	FORCEINLINE bool ShouldJumpStart() const { return IsFalling; }
	UFUNCTION()
	FORCEINLINE bool ShouldJumpLoop() const { return IsFalling; }
	UFUNCTION()
	FORCEINLINE bool ShouldJumpEnd() const { return !IsFalling; }
	UFUNCTION()
	FORCEINLINE bool ShouldSprint() const { return IsSprinting; }
	UFUNCTION()
	FORCEINLINE bool ShouldTargeting() const { return IsTargeting; }

	UFUNCTION()
	void StandEntered(const FAnimNode_StateMachine& Machine, int32 PrevStateIndex, int32 NextStateIndex) {}
	UFUNCTION()
	void CrouchEntered(const FAnimNode_StateMachine& Machine, int32 PrevStateIndex, int32 NextStateIndex) {}
	UFUNCTION()
	void JumpStartEntered(const FAnimNode_StateMachine& Machine, int32 PrevStateIndex, int32 NextStateIndex) {}
	UFUNCTION()
	void JumpLoopEntered(const FAnimNode_StateMachine& Machine, int32 PrevStateIndex, int32 NextStateIndex) {}
	UFUNCTION()
	void JumpEndEntered(const FAnimNode_StateMachine& Machine, int32 PrevStateIndex, int32 NextStateIndex) {}
	UFUNCTION()
	void SprintEntered(const FAnimNode_StateMachine& Machine, int32 PrevStateIndex, int32 NextStateIndex) {}
	UFUNCTION()
	void TargetingEntered(const FAnimNode_StateMachine& Machine, int32 PrevStateIndex, int32 NextStateIndex) {}

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Animation)
	float Speed;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Animation)
	float Direction;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Animation)
	bool IsFalling;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Animation)
	bool IsCrouching;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Animation)
	bool IsSprinting;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Animation)
	bool IsTargeting;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Animation)
	float AimOffsetYaw;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Animation)
	float AimOffsetPitch;
};
