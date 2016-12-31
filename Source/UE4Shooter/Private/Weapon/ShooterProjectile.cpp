// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterProjectile.h"

#include "UnrealNetwork.h"

#include "ShooterImpactEffect.h"

AShooterProjectile::AShooterProjectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	SetReplicates(true);
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicateMovement = true;

	//!< スフィア
	SphereComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, "SphereComp");
	if (nullptr != SphereComp)
	{
		SphereComp->InitSphereRadius(5.0f);
		SphereComp->AlwaysLoadOnClient = true;
		SphereComp->AlwaysLoadOnServer = true;
		SphereComp->bTraceComplexOnMove = true;

		//!< コリジョン
		SphereComp->SetCollisionObjectType(ECC_GameTraceChannel_WeaponProjectile);
		SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		SphereComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		SphereComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

		SetRootComponent(SphereComp);
	}

	//!< メッシュ
	StaticMeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "StaticMeshComp");
	if (nullptr != StaticMeshComp)
	{
		//!< メッシュ自体のコリジョンは無し
		StaticMeshComp->SetCollisionObjectType(ECC_WorldDynamic);
		StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StaticMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);

		StaticMeshComp->SetupAttachment(SphereComp);
	}

	//!< プロジェクタイル
	ProjectileMovementComp = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, "ProjectileMovementComp");
	if (nullptr != ProjectileMovementComp)
	{
		ProjectileMovementComp->InitialSpeed = 2000.0f;
		ProjectileMovementComp->MaxSpeed = 2000.0f;
		ProjectileMovementComp->bRotationFollowsVelocity = true;
		ProjectileMovementComp->ProjectileGravityScale = 0.0f;

		//!< ホーミング設定
		//ProjectileMovementComp->bIsHomingProjectile = true;
		//ProjectileMovementComp->HomingAccelerationMagnitude = ;
		//ProjectileMovementComp->HomingTargetComponent = nullptr;

		ProjectileMovementComp->SetUpdatedComponent(SphereComp);
	}
}

void AShooterProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (nullptr != SphereComp)
	{
		SphereComp->MoveIgnoreActors.Add(Instigator);
	}
	if (nullptr != ProjectileMovementComp)
	{
		ProjectileMovementComp->OnProjectileStop.AddDynamic(this, &AShooterProjectile::OnProjectileStop);
	}
	if (nullptr != TrailParticle)
	{
		TrailParticleComp = UGameplayStatics::SpawnEmitterAttached(TrailParticle, StaticMeshComp);
	}

	SetLifeSpan(10.0f);
}
void AShooterProjectile::PostNetReceiveVelocity(const FVector& NewVelocity)
{
	Super::PostNetReceiveVelocity(NewVelocity);

	if (nullptr != ProjectileMovementComp)
	{
		ProjectileMovementComp->Velocity = NewVelocity;
	}
}

void AShooterProjectile::OnProjectileStop(const FHitResult& HitResult)
{
	if (HasAuthority())
	{
		if (false == bExploded)
		{
			//!< レプリケートされてクライアントで SimulateExplode() がコールされる
			bExploded = true;

			//!< ダメージ
			const auto Location = HitResult.ImpactPoint + HitResult.ImpactNormal * 10.0f;
			const auto Damage = 100.0f;
			const auto Radius = 100.0f;
			UGameplayStatics::ApplyRadialDamage(this, Damage, Location, Radius, UShooterDamageType::StaticClass(), TArray<AActor*>(), this, GetInstigatorController());

			if (nullptr != ProjectileMovementComp)
			{
				ProjectileMovementComp->StopMovementImmediately();
			}

			SetLifeSpan(2.0f);
		}
	}
}
void AShooterProjectile::OnRep_Exploded()
{
	const auto World = GetWorld();
	if (nullptr != World)
	{
		//!< ヒットした場所を推測する
		const auto Forward = GetActorForwardVector();
		const auto Location = GetActorLocation();
		const auto Start = Location - Forward * 200.0f;
		const auto End = Location + Forward * 150.0f;

		FHitResult HitResult;
		const FCollisionQueryParams Params(TEXT("ExplodeTag"), true, Instigator);
		if (false == World->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel_WeaponProjectile, Params))
		{
			//!< コリジョンが取れない場合でもそれっぽい値にしておく
			HitResult.ImpactPoint = Location;
			HitResult.ImpactNormal = -Forward;
		}

		SimulateExplode(HitResult);
	}
}
void AShooterProjectile::SimulateExplode(const FHitResult& HitResult)
{
	if (nullptr != StaticMeshComp)
	{
		StaticMeshComp->SetHiddenInGame(true);
	}
	if (nullptr != TrailParticleComp)
	{
		TrailParticleComp->SetHiddenInGame(true);
	}
	
	SpawnImpactEffect(HitResult);
}

void AShooterProjectile::SpawnImpactEffect(const FHitResult& HitResult)
{
	AShooterWeapon::SpawnImpactEffect(GetWorld(), ImpactEffectClass, HitResult);
}

void AShooterProjectile::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterProjectile, bExploded);
}