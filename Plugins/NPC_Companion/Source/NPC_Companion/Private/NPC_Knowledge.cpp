// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Knowledge.h"
#include "HAL/FileManager.h"
#include "Kismet/GameplayStatics.h"

void UNPC_Knowledge::SaveInteractionData(const FPlayerInteractionData& InteractionData,const FString& NPCName)
{
	IFileManager::Get().MakeDirectory(*NPCSaveFolder, true);

	FString FullPath = NPCSaveFolder / NPCName;

	if (UNPC_Knowledge* SaveGameInstance = Cast<UNPC_Knowledge>(UGameplayStatics::CreateSaveGameObject(UNPC_Knowledge::StaticClass())))
	{
		SaveGameInstance->PlayerInteractions = InteractionData;
		if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, FullPath, 0))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save game to slot: %s"), *FullPath);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Game saved to: %s"), *FullPath);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create SaveGameInstance."));
	}
}

FPlayerInteractionData UNPC_Knowledge::LoadInteractionData(const FString& NPCName)
{
	FString FullPath = NPCSaveFolder / NPCName;

	if (UNPC_Knowledge* LoadedGame = Cast<UNPC_Knowledge>(UGameplayStatics::LoadGameFromSlot(FullPath, 0)))
	{
		return LoadedGame->PlayerInteractions;
	}

	// Return default interaction data if none is found
	return FPlayerInteractionData();
}

