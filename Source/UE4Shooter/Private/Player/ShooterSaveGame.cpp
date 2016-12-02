// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterSaveGame.h"

#include "Player/ShooterLocalPlayer.h"

void UShooterSaveGame::Save()
{
	UGameplayStatics::SaveGameToSlot(this, SlotName, UserIndex);
}
UShooterSaveGame* UShooterSaveGame::Load(const FString& SlotName, const int32 UserIndex)
{
	if (0 < SlotName.Len())
	{
		auto SaveGame = Cast<UShooterSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
		if (nullptr == SaveGame)
		{
			//!< ƒ[ƒh‚Å‚«‚È‚¢ê‡‚ÍV‹Kì¬
			SaveGame = Cast<UShooterSaveGame>(UGameplayStatics::CreateSaveGameObject(UShooterSaveGame::StaticClass()));
		}
		check(nullptr != SaveGame);

		SaveGame->SlotName = SlotName;
		SaveGame->UserIndex = UserIndex;
		
		return SaveGame;
	}
	return nullptr;
}
void UShooterSaveGame::TellInputAboutKeybindings()
{
	if (nullptr != GEngine)
	{
		TArray<APlayerController*> PlayerControllers;
		GEngine->GetAllLocalPlayerControllers(PlayerControllers);

		for (auto It = PlayerControllers.CreateIterator(); It; ++It)
		{
			const auto PC = *It;
			if (nullptr != PC && nullptr != PC->Player)
			{
				const auto LP = Cast<UShooterLocalPlayer>(PC->Player);
				if (nullptr != LP && this != LP->GetSaveGame())
				{
					if (nullptr != PC->PlayerInput)
					{
						for (auto i : PC->PlayerInput->AxisMappings)
						{
							if ("Lookup" == i.AxisName || "Turn" == i.AxisName)
							{
								i.Scale = (i.Scale < 0.0f) ? -GetAimSensitivity() : GetAimSensitivity();
							}
						}
						
						PC->PlayerInput->ForceRebuildingKeyMaps();
						if (PC->PlayerInput->GetInvertAxis("Lookup") != GetInvertedYAxis())
						{
							PC->PlayerInput->InvertAxis("Lookup");
						}
					}
				}
			}	
		}
	}
}
