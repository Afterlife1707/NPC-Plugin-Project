// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Misc/Paths.h"
#include "NPC_Knowledge.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPlayerInteractionData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, Category="Interaction Data")
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite, Category="Interaction Data")
	TArray<FString> Messages;

	FString ToString() const
	{
		FString Result;
		for (const FString& Message : Messages)
		{
			Result += Message + "\n";
		}
		return Result;
	}
};

UCLASS()
class NPC_COMPANION_API UNPC_Knowledge : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="Save Data")
	FPlayerInteractionData PlayerInteractions;

	UFUNCTION(BlueprintCallable, Category="SaveLoad")
	void SaveInteractionData(const FPlayerInteractionData& InteractionData, const FString& NPCName);

	UFUNCTION(BlueprintCallable, Category="SaveLoad")
	FPlayerInteractionData LoadInteractionData(const FString& NPCName);
	
	FString NPCSaveFolder = FPaths::ProjectSavedDir() / TEXT("SaveGames/NPCs");
};
