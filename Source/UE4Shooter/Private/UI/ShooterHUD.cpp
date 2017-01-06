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
		const auto Character = Cast<AShooterCharacter>(GetOwningPawn());
		if (nullptr != Character)
		{
			const auto Weapon = Character->GetWeapon();
			if (nullptr != Weapon)
			{
				{
					const auto Location = FVector2D(Canvas->ClipX * 0.1f, Canvas->ClipY * 0.1f);
					
					const auto HealthString = FString::SanitizeFloat(Character->GetHealth());
					DrawText(TEXT("Health : ") + HealthString, FColor::Green, Location.X, Location.Y);

					const auto AmmoString = TEXT("(") + FString::FromInt(Weapon->GetAmmoInClip()) + TEXT(" / ") + FString::FromInt(Weapon->GetAmmoPerClip()) + TEXT(") ") + FString::FromInt(Weapon->GetAmmo());
					DrawText(TEXT("Ammo : ") + AmmoString, FColor::Yellow, Location.X, Location.Y + 16.0f);
				}

				const auto CrosshairTexture = Weapon->GetCrosshairTexture();
				if (nullptr != CrosshairTexture)
				{
					const auto Location = FVector2D(Canvas->ClipX, Canvas->ClipY) * 0.5f;

					const auto Offset2D = FVector2D(static_cast<float>(CrosshairTexture->GetSizeX() >> 1), static_cast<float>(CrosshairTexture->GetSizeY() >> 1));
					auto CrosshairItem = FCanvasTileItem(Location - Offset2D, CrosshairTexture->Resource, FLinearColor::White);
					CrosshairItem.BlendMode = SE_BLEND_Translucent;
					Canvas->DrawItem(CrosshairItem);
				}
			}
		}
	}
}
