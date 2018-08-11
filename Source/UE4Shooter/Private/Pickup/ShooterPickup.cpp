// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterPickup.h"

#include "UnrealNetwork.h"

AShooterPickup::AShooterPickup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetReplicates(true);
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);

	auto CapsuleComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("CapsuleComp"));
	if (nullptr != CapsuleComp)
	{
		CapsuleComp->InitCapsuleSize(20.0f, 30.0f);
		CapsuleComp->SetCollisionObjectType(ECC_GameTraceChannel_Pickup);
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		RootComponent = CapsuleComp;
	}

	StaticMeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMeshComp"));
	if (nullptr != StaticMeshComp)
	{
		StaticMeshComp->bReceivesDecals = false;
		StaticMeshComp->SetupAttachment(CapsuleComp);
	}
}

void AShooterPickup::BeginPlay()
{
	Super::BeginPlay();

	Respawn();
}
void AShooterPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (bIsActive && !IsPendingKill())
	{
		if (GiveTo(OtherActor))
		{
			PickedUp();
		}
	}
}

void AShooterPickup::OnRep_IsActive()
{
	if (bIsActive)
	{
		SimulateRespawn();
	}
	else
	{
		SimulatePickedUp();
	}
}

void AShooterPickup::Respawn()
{
	if (HasAuthority())
	{
		bIsActive = true;

		//!< ���X�|�[����������ɏd�Ȃ��Ă���ꍇ������
		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors, AActor::StaticClass());
		for (auto i : OverlappingActors)
		{
			if (GiveTo(i))
			{
				PickedUp();
			}
		}
	}
	else
	{
		SimulateRespawn();
	}
}

void AShooterPickup::PickedUp()
{
	if (HasAuthority())
	{
		if (bIsActive)
		{
			bIsActive = false;
		}

		//!< ���X�|�[������
		GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &AShooterPickup::Respawn, 5.0f, false);
		//!< ����������
		//SetLifeSpan(1.0f);
	}
}

void AShooterPickup::SimulatePickedUp()
{
	if (nullptr != StaticMeshComp)
	{
		StaticMeshComp->SetHiddenInGame(true);
	}
	if (nullptr != PickupSoundCue)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSoundCue, GetActorLocation());
	}
}
void AShooterPickup::SimulateRespawn()
{
	if (nullptr != StaticMeshComp)
	{
		StaticMeshComp->SetHiddenInGame(false);
	}

	DrawDebugString(GetWorld(), FVector::UpVector * 50.0f, GetClass()->GetName(), this, FColor::Green, FLT_MAX);
}

void AShooterPickup::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterPickup, bIsActive);
}