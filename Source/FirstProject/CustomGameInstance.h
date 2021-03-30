// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "PlayerData")
	float Health;

	UPROPERTY(VisibleAnywhere, Category = "PlayerData")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "PlayerData")
	float Stamina;

	UPROPERTY(VisibleAnywhere, Category = "PlayerData")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, Category = "PlayerData")
	int32 Coins;

	UPROPERTY(VisibleAnywhere, Category = "PlayerData")
	FString WeaponName;
};

USTRUCT(BlueprintType)
struct FSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FString WeaponName;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FString MapName;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FVector Location;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FRotator Rotation;
};

UCLASS()
class FIRSTPROJECT_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UCustomGameInstance();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bSetPlayerData;
	bool bCanLoad;
	bool bGameLoaded;

	void SetPlayerData();

	void SetGameInstanceData();

	UFUNCTION(BlueprintCallable)
	void SaveGame();
	UFUNCTION(BlueprintCallable)
	void LoadGame();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AItemStorage> WeaponStorage;

	FPlayerData PlayerData;
	FSaveData SaveData;
};
