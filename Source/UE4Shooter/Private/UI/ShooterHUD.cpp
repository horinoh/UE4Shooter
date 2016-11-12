// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterHUD.h"

#include "Pawn/ShooterCharacter.h"
#include "Weapon/ShooterWeapon.h"

void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();

	if (nullptr != Canvas)
	{
		const auto Location2D = FVector2D(Canvas->ClipX, Canvas->ClipY) * 0.5f;
		const auto Location = FVector(Location2D.X, Location2D.Y, 0.0f);

		const auto Character = Cast<AShooterCharacter>(GetOwningPawn());
		if (nullptr != Character)
		{
			const auto Weapon = Character->GetWeapon();
			if (nullptr != Weapon)
			{
				const auto CrosshairTexture = Weapon->GetCrosshairTexture();
				if (nullptr != CrosshairTexture)
				{
					const auto Offset2D = FVector2D(static_cast<float>(CrosshairTexture->GetSizeX() >> 1), static_cast<float>(CrosshairTexture->GetSizeY() >> 1));
					auto CrosshairItem = FCanvasTileItem(Location2D - Offset2D, CrosshairTexture->Resource, FLinearColor::White);
					CrosshairItem.BlendMode = SE_BLEND_Translucent;
					Canvas->DrawItem(CrosshairItem);
				}
			}
		}
	}
}
