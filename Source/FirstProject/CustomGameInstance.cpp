// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameInstance.h"
#include "FirstSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GuardCharacter.h"
#include "Weapon.h"
#include "ItemStorage.h"


UCustomGameInstance::UCustomGameInstance()
{
	bSetPlayerData = false;
	bCanLoad = false;
	bGameLoaded = false;
}

void UCustomGameInstance::SetPlayerData()
{
	AGuardCharacter* Main = Cast<AGuardCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Main)
	{
		Main->Health = PlayerData.Health;
		Main->MaxHealth = PlayerData.MaxHealth;
		Main->Stamina = PlayerData.Stamina;
		Main->MaxStamina = PlayerData.MaxStamina;
		Main->Coins = PlayerData.Coins;

		if (WeaponStorage)
		{
			AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);

			if (Weapons)
			{
				if (Weapons->WeaponMap.Contains(PlayerData.WeaponName))
				{
					AWeapon* WeaponToSpawn = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[PlayerData.WeaponName]);
					WeaponToSpawn->Equip(Main);
				}
			}
		}

		if (bGameLoaded)
		{
			Main->SetActorLocation(SaveData.Location);
			Main->SetActorRotation(SaveData.Rotation);
		}

		bSetPlayerData = false;
	}
}

void UCustomGameInstance::SetGameInstanceData()
{
	AGuardCharacter* Main = Cast<AGuardCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Main)
	{
		PlayerData.Health = Main->Health;
		PlayerData.MaxHealth = Main->MaxHealth;
		PlayerData.Stamina = Main->Stamina;
		PlayerData.MaxStamina = Main->MaxStamina;
		PlayerData.Coins = Main->Coins;

		// this function is called from thetransition volume,
		// so when the player saves the game and then travels to a different level, 
		// all the data gets overwritten (not the position and rotation tho)
		if (Main->EquippedWeapon)
		{
			PlayerData.WeaponName = Main->EquippedWeapon->Name;
		}
	}
}

void UCustomGameInstance::SaveGame()
{
	AGuardCharacter* Main = Cast<AGuardCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Main)
	{
		FString MapToSave = GetWorld()->GetMapName();
		MapToSave.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
		SaveData.MapName = MapToSave;
		SaveData.Location = Main->GetActorLocation();
		SaveData.Rotation = Main->GetActorRotation();

		SetGameInstanceData();
		bCanLoad = true;
	}
}

void UCustomGameInstance::LoadGame()
{
	AGuardCharacter* Main = Cast<AGuardCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Main && bCanLoad)
	{
		FString MapToLoad = SaveData.MapName;
		FString CurrentMap = GetWorld()->GetMapName();
		CurrentMap.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

		if (MapToLoad != CurrentMap && MapToLoad != TEXT(""))
		{
			bGameLoaded = true;
			FName MapName(MapToLoad);
			Main->SwitchLevel(MapName);
			bSetPlayerData = true;
		}
		else
		{
			SetPlayerData();
		}

		Main->SetActorLocation(SaveData.Location);
		Main->SetActorRotation(SaveData.Rotation);
	}
}
