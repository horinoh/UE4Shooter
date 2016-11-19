// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "Projectile_GrenadeLauncher.h"

AProjectile_GrenadeLauncher::AProjectile_GrenadeLauncher(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (nullptr != StaticMeshComp)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/PrototypeWeap/Prototype_GrenadeLauncher_Ammo.Prototype_GrenadeLauncher_Ammo'"));
		if (StaticMesh.Succeeded())
		{
			StaticMeshComp->SetStaticMesh(StaticMesh.Object);
		}
		StaticMeshComp->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	}
}
