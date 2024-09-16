// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WorldSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class NPC_COMPANION_API UWorldSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGameData")
	FString CommonKnowledge; // Stores the dynamic event data
};
