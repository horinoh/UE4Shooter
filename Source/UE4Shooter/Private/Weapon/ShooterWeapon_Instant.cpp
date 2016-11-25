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

	FVector Start, Direction;
	GetAim(Start, Direction);
	const auto MuzzleLocation = GetMuzzleLocation();
	
	const auto SpreadNum = GetSpreadNum();
	TArray<FHitResult> HitResults;
	HitResults.Reserve(32);
	check(SpreadNum <= HitResults.Max());
	uint32 MissResults = 0;
	for (auto i = 0; i < SpreadNum; ++i)
	{
		const auto SpreadDirection = RandomStream.VRandCone(Direction, ConeHalfRadian, ConeHalfRadian);
		const auto End = Start + RangeDistance * SpreadDirection;
		FHitResult HitResult(ForceInit);
		if (LineTraceWeapon(Start, End, HitResult))
		{
			HitResults.Add(HitResult);

			const auto World = GetWorld();
			if (nullptr != World)
			{
				const auto Location = HitResult.ImpactPoint;
				if (nullptr != ImpactEffectClass)
				{
					const auto Rotation = HitResult.ImpactNormal.Rotation();

					auto Effect = World->SpawnActorDeferred<AShooterImpactEffect>(ImpactEffectClass, FTransform(Rotation, Location));
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

				DrawDebugLine(World, MuzzleLocation, HitResult.ImpactPoint, FColor::Red, false, 2.0f);
			}
		}
		else
		{
			MissResults |= (1 << i);

			DrawDebugLine(GetWorld(), MuzzleLocation, End, FColor::Red, false, 2.0f);
		}
	}
	if (HitResults.Num())
	{
		Hits(HitResults);
	}
	if (MissResults)
	{
		Misses(RandomSeed, MissResults);
	}
}
void AShooterWeapon_Instant::OnEquipFinished()
{
	Super::OnEquipFinished();

	FireNotify.EndLocations.Empty();
}

void AShooterWeapon_Instant::HitConfirmed(const FHitResult& HitResult)
{
	const auto Actor = HitResult.GetActor();
	if (nullptr != Actor)
	{
		//!< #TODO
		//const auto Pawn = Cast<APawn>(GetOwner());
		//if (nullptr != Pawn)
		//{
		//	FPointDamageEvent DamageEvent;
		//	DamageEvent.DamageTypeClass = UShooterDamageType::StaticClass();
		//	DamageEvent.Damage = 10.0f;
		//	DamageEvent.ShotDirection = (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal();
		//	DamageEvent.HitInfo = HitResult;

		//	UE_LOG(LogShooter, Log, TEXT("%s : TakeDamage %s -> %s ( %s )"), HasAuthority() ? TEXT("Server") : TEXT("Client"), *Pawn->GetName(), *Actor->GetName(), *HitResult.BoneName.ToString());
		//	if (TEXT("head") == HitResult.BoneName)
		//	{
		//		DamageEvent.Damage *= 10.0f;
		//	}
		//	Actor->TakeDamage(DamageEvent.Damage, DamageEvent, Pawn->GetController(), this);
		//}
	}

	//!< 要素数が SpreadNum を超えてたということは、前回の発砲が完了したのでリセット
	const auto SpreadNum = GetSpreadNum();
	if (SpreadNum <= FireNotify.EndLocations.Num())
	{
		FireNotify.EndLocations.Empty();
	}
	FireNotify.EndLocations.Add(HitResult.ImpactPoint);
}

void AShooterWeapon_Instant::Hits(const TArray<FHitResult>& HitResults)
{
	if (HasAuthority())
	{
		for (const auto i : HitResults)
		{
			const auto Actor = i.GetActor();
			if (nullptr == Actor)
			{
				if (i.IsValidBlockingHit())
				{
					//!< アクタ以外にヒット → 確定で良い
					HitConfirmed(i);
				}
			}
			else if (Actor->IsRootComponentStatic() || Actor->IsRootComponentStationary())
			{
				//!< 静的なアクタにヒット → 確定で良い (ゲームプレイには影響しない)
				HitConfirmed(i);
			}
			else
			{
				const auto Box = Actor->GetComponentsBoundingBox();
				auto Extent = (Box.Max - Box.Min) * 0.5f;
				//!< バウンディングボックスが薄すぎる場合の精度エラー対策
				Extent.X = FMath::Max(Extent.X, 20.0f);
				Extent.Y = FMath::Max(Extent.Y, 20.0f);
				Extent.Z = FMath::Max(Extent.Z, 20.0f);
				const auto Center = (Box.Max + Box.Min) * 0.5f;
				if (FMath::Abs(i.Location.Z - Center.Z) < Extent.Z &&
					FMath::Abs(i.Location.X - Center.X) < Extent.X &&
					FMath::Abs(i.Location.Y - Center.Y) < Extent.Y)
				{
					//!< サーバでバウンディングボックスにヒット → 確定で良い
					HitConfirmed(i);
				}
			}
		}
	}
	else
	{
		ServerHits(HitResults);
	}
}
bool AShooterWeapon_Instant::ServerHits_Validate(const TArray<FHitResult> HitResults)
{
	return true;
}
void AShooterWeapon_Instant::ServerHits_Implementation(const TArray<FHitResult> HitResults)
{
	Hits(HitResults);
}

void AShooterWeapon_Instant::Misses(const int32 RandomSeed, const uint32 MissResults)
{
	if (HasAuthority())
	{
		auto RandomStream = FRandomStream(RandomSeed);

		const auto SpreadAngle = GetSpreadAngle();
		const auto ConeHalfRadian = FMath::DegreesToRadians(SpreadAngle * 0.5f);
		const auto RangeDistance = GetRangeDistance();

		FVector Start, Direction;
		GetAim(Start, Direction);

		const auto SpreadNum = GetSpreadNum();
		for (auto i = 0; i < SpreadNum; ++i)
		{
			const auto SpreadDirection = RandomStream.VRandCone(Direction, ConeHalfRadian, ConeHalfRadian);

			if (MissResults & (1 << i))
			{
				const auto End = Start + RangeDistance * SpreadDirection;

				//!< 要素数が SpreadNum を超えてたということは、前回の発砲が完了したのでリセット
				if (SpreadNum <= FireNotify.EndLocations.Num())
				{
					FireNotify.EndLocations.Empty();
				}
				FireNotify.EndLocations.Add(End);
			}
		}
	}
	else
	{
		ServerMisses(RandomSeed, MissResults);
	}
}
bool AShooterWeapon_Instant::ServerMisses_Validate(const int32 RandomSeed, const uint32 MissResults)
{
	return true;
}
void AShooterWeapon_Instant::ServerMisses_Implementation(const int32 RandomSeed, const uint32 MissResults)
{
	Misses(RandomSeed, MissResults);
}

void AShooterWeapon_Instant::OnRep_FireNotify()
{
	for (const auto& i : FireNotify.EndLocations)
	{
		const auto MuzzleLocation = GetMuzzleLocation();
		const auto Start = MuzzleLocation;
		const auto Direction = (i - Start).GetSafeNormal();
		const auto RangeDistance = GetRangeDistance();
		const auto End = Start + RangeDistance * Direction;

		FHitResult HitResult(ForceInit);
		if (LineTraceWeapon(Start, End, HitResult))
		{
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 8, FColor::Red, false, 2.0f);
			DrawDebugLine(GetWorld(), MuzzleLocation, HitResult.ImpactPoint, FColor::Red, false, 2.0f);
		}
		else
		{
			DrawDebugLine(GetWorld(), MuzzleLocation, End, FColor::Red, false, 2.0f);
		}
	}
}

void AShooterWeapon_Instant::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AShooterWeapon_Instant, FireNotify, COND_SkipOwner);
}
