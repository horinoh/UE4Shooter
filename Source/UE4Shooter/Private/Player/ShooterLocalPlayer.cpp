// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4Shooter.h"
#include "ShooterLocalPlayer.h"

#include "OnlineSubsystemUtils.h"

#include "ShooterSaveGame.h"

void UShooterLocalPlayer::SetControllerId(int32 NewControllerId)
{
	Super::SetControllerId(NewControllerId);

	LoadSaveGame();
}

FString UShooterLocalPlayer::GetNickname() const
{
	auto Nickname = Super::GetNickname();

	auto bReplace = (Nickname.Len() == 0);

	static bool bReentry = false;
	if (!bReentry)
	{
		bReentry = true;

		const auto World = GetWorld();
		if (nullptr != World)
		{
			const auto GameInstance = World->GetGameInstance();
			if (nullptr != GameInstance)
			{
				const auto LocalPlayers = GameInstance->GetLocalPlayers();
				for (auto i : LocalPlayers)
				{
					if (this == i) { break; }
					if (i->GetNickname() == Nickname)
					{
						bReplace = true;
						break;
					}
				}
			}
		}
		bReentry = false;
	}

	if (bReplace)
	{
		Nickname = FString::Printf(TEXT("Player%i"), GetControllerId() + 1);
	}

	return Nickname;
}

class UShooterSaveGame* UShooterLocalPlayer::GetSaveGame() const
{
	if (nullptr == SaveGame)
	{
		const_cast<UShooterLocalPlayer*>(this)->LoadSaveGame();
	}
	return SaveGame;
}
void UShooterLocalPlayer::LoadSaveGame()
{
	auto SlotName = GetNickname();
	auto UserIndex = GetControllerId();

//#if PLATFORM_WOLF
//	FPlatformMisc::GetUniqueStringNameForControllerId(UserIdex, SaveGameName);
//#endif

	if (nullptr != SaveGame)
	{
		if (SaveGame->GetUserIndex() != UserIndex || SaveGame->GetSlotName() != SlotName)
		{
			SaveGame->SaveIfDirty();
			SaveGame = nullptr;
		}
	}

	if (nullptr == SaveGame)
	{
		const auto IdentityInterface = Online::GetIdentityInterface();
		if (IdentityInterface.IsValid() && GetPreferredUniqueNetId().IsValid())
		{
			UserIndex = IdentityInterface->GetPlatformUserIdFromUniqueNetId(*GetPreferredUniqueNetId());
		}

		SaveGame = UShooterSaveGame::Load(SlotName, UserIndex);
	}
}
