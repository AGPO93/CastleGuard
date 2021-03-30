// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FirstSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStats
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float Health;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float Stamina;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 Coins;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FVector Location;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FRotator Rotation;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FString WeaponName;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FString MapName;
};

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API UFirstSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UFirstSaveGame();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 SlotIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FPlayerStats PlayerStats;
};
