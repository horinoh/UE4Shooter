// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "Projectile_RocketLauncher.h"

AProjectile_RocketLauncher::AProjectile_RocketLauncher(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (nullptr != StaticMeshComp)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/PrototypeWeap/Prototype_RocketLauncher_Ammo.Prototype_RocketLauncher_Ammo'"));
		if (StaticMesh.Succeeded())
		{
			StaticMeshComp->SetStaticMesh(StaticMesh.Object);
		}
		StaticMeshComp->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	}
}
