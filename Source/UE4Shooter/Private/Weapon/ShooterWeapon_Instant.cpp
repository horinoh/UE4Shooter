// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterWeapon_Instant.h"

#include "UnrealNetwork.h"

void AShooterWeapon_Instant::Fire()
{
	const auto RandomSeed = FMath::Rand();
	auto RandomStream = FRandomStream(RandomSeed);

	const auto SpreadAngle = GetSpreadAngle();
	const auto ConeHalfRadian = FMath::DegreesToRadians(SpreadAngle * 0.5f);
	const auto RangeDistance = GetRangeDistance();

	FVector Origin, Direction;
	GetAim(Origin, Direction);
	const auto MuzzleLocation = GetMuzzleLocation();

	const auto SpreadNum = GetSpreadNum();
	TArray<FHitResult> HitResults;
	HitResults.Reserve(8);
	check(SpreadNum <= HitResults.Max());
	for (auto i = 0; i < SpreadNum; ++i)
	{
		const auto SpreadDirection = RandomStream.VRandCone(Direction, ConeHalfRadian, ConeHalfRadian);
		const auto End = Origin + RangeDistance * SpreadDirection;
		FHitResult HitResult(ForceInit);
		if (LineTraceWeapon(Origin, End, HitResult))
		{
			HitResults.Add(HitResult);

			SpawnImpactEffect(HitResult);
			SpawnTrailEffect(HitResult.ImpactPoint);
		}
		else
		{
			SpawnTrailEffect(End);
		}
	}

	Hits(Origin, RandomSeed, HitResults);
}

void AShooterWeapon_Instant::HitConfirmed(const FHitResult& HitResult)
{
	const auto Actor = HitResult.GetActor();
	if (nullptr != Actor)
	{
		const auto Pawn = Cast<APawn>(GetOwner());
		if (nullptr != Pawn)
		{
			FPointDamageEvent DamageEvent;
			DamageEvent.DamageTypeClass = UDamageType::StaticClass();//UShooterDamageType::StaticClass();
			DamageEvent.Damage = 10.0f;
			DamageEvent.ShotDirection = (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal();
			DamageEvent.HitInfo = HitResult;

			if (TEXT("head") == HitResult.BoneName)
			{
				DamageEvent.Damage *= 10.0f;
			}
			Actor->TakeDamage(DamageEvent.Damage, DamageEvent, Pawn->GetController(), this);
		}
	}
}

void AShooterWeapon_Instant::Hits(const FVector Start, const int32 RandomSeed, const TArray<FHitResult>& HitResults)
{
	if (HasAuthority())
	{
		if (nullptr != Instigator)
		{
			//!< �� OnRep_FireNotify()
			FireNotify.Origin = Start;
			FireNotify.RandomSeed = RandomSeed;

			//!< (��������)���했�̋��e����
			const auto ReticleDot = FMath::Abs(FMath::Sin(FMath::DegreesToRadians(10.0f)));
			//!< �L�����Ǝː��̋��e���� (���̒l�ȏ�Ȃ�����������Ă���Ƃ݂Ȃ�)
			const auto AllowedDot = 0.8f - ReticleDot;

			const auto ViewDir = Instigator->GetViewRotation().Vector();

			for (const auto& i : HitResults)
			{
				//!< �L�����Ǝː����T�ˍ����Ă��邱�Ƃ��`�F�b�N
				const auto LineTraceDir = (i.ImpactPoint - GetMuzzleLocation()).GetSafeNormal();
				if (FVector::DotProduct(ViewDir, LineTraceDir) > AllowedDot)
				{
					const auto Actor = i.GetActor();
					if (nullptr == Actor)
					{
						if (i.IsValidBlockingHit())
						{
							//!< �A�N�^�ȊO�Ƀq�b�g �� �m��ŗǂ�
							HitConfirmed(i);
						}
					}
					else if (Actor->IsRootComponentStatic() || Actor->IsRootComponentStationary())
					{
						//!< �ÓI�ȃA�N�^�Ƀq�b�g �� �m��ŗǂ� (�Q�[���v���C�ɂ͉e�����Ȃ�)
						HitConfirmed(i);
					}
					else
					{
						const auto Box = Actor->GetComponentsBoundingBox();
						auto Extent = (Box.Max - Box.Min) * 0.5f;
						//!< �o�E���f�B���O�{�b�N�X����������ꍇ�̐��x�G���[�΍�
						Extent.X = FMath::Max(Extent.X, 20.0f);
						Extent.Y = FMath::Max(Extent.Y, 20.0f);
						Extent.Z = FMath::Max(Extent.Z, 20.0f);
						const auto Center = (Box.Max + Box.Min) * 0.5f;
						if (FMath::Abs(i.Location.Z - Center.Z) < Extent.Z &&
							FMath::Abs(i.Location.X - Center.X) < Extent.X &&
							FMath::Abs(i.Location.Y - Center.Y) < Extent.Y)
						{
							//!< �T�[�o�Ńo�E���f�B���O�{�b�N�X�Ƀq�b�g �� �m��ŗǂ�
							HitConfirmed(i);
						}
					}
				}
			}
		}
	}
	else
	{
		ServerHits(Start, RandomSeed, HitResults);
	}
}
bool AShooterWeapon_Instant::ServerHits_Validate(const FVector Start, const int32 RandomSeed, const TArray<FHitResult> HitResults)
{
	return true;
}
void AShooterWeapon_Instant::ServerHits_Implementation(const FVector Start, const int32 RandomSeed, const TArray<FHitResult> HitResults)
{
	Hits(Start, RandomSeed, HitResults);
}

void AShooterWeapon_Instant::OnRep_FireNotify()
{
	auto RandomStream = FRandomStream(FireNotify.RandomSeed);

	const auto SpreadAngle = GetSpreadAngle();
	const auto ConeHalfRadian = FMath::DegreesToRadians(SpreadAngle * 0.5f);
	const auto RangeDistance = GetRangeDistance();

	const auto Origin = FireNotify.Origin;
	const auto Direction = GetAimDirection();

	const auto SpreadNum = GetSpreadNum();
	for (auto i = 0; i < SpreadNum; ++i)
	{
		const auto SpreadDirection = RandomStream.VRandCone(Direction, ConeHalfRadian, ConeHalfRadian);
		const auto End = Origin + RangeDistance * SpreadDirection;

		FHitResult HitResult(ForceInit);
		if (LineTraceWeapon(Origin, End, HitResult))
		{
			SpawnImpactEffect(HitResult);
			SpawnTrailEffect(HitResult.ImpactPoint);
		}
		else
		{
			SpawnTrailEffect(End);
		}
	}
}

void AShooterWeapon_Instant::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AShooterWeapon_Instant, FireNotify, COND_SkipOwner);
}
