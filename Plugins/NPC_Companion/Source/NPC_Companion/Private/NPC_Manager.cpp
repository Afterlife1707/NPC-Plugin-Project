// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Manager.h"

#include "NPC_Base.h"
#include "WorldSaveGame.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANPC_Manager::ANPC_Manager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANPC_Manager::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(this, ANPC_Base::StaticClass(),RegisteredNPCs);
	LoadWorldData();
	for (auto& NPCActor : RegisteredNPCs)
	{
		if (ANPC_Base* NPC = Cast<ANPC_Base>(NPCActor))
			NPC_Names+=NPC->GetNPC_Name()+TEXT(",");
	}
}

// Called every frame
void ANPC_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC_Manager::DynamicEvent(const FString& EventDescription)
{
	CommonKnowledge+=TEXT(".")+EventDescription;
	SaveWorldData();
	for (auto& NPCActor : RegisteredNPCs) //notify all npcs
	{
		if (ANPC_Base* NPC = Cast<ANPC_Base>(NPCActor))
		{
			NPC->UpdateKnowledge(EventDescription);
		}
	}
}

void ANPC_Manager::SaveWorldData()
{
	if (UWorldSaveGame* SaveGameInstance = Cast<UWorldSaveGame>(UGameplayStatics::CreateSaveGameObject(UWorldSaveGame::StaticClass())))
	{
		SaveGameInstance->CommonKnowledge = CommonKnowledge;

		IFileManager::Get().MakeDirectory(*WorldSaveFolder, true);
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, FullPath, 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("World Data Saved to: %s"), *FullPath);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save world data to slot: %s"), *FullPath);
		}
	}
}

void ANPC_Manager::LoadWorldData()
{
	if (UWorldSaveGame* LoadGameInstance = Cast<UWorldSaveGame>(UGameplayStatics::LoadGameFromSlot(FullPath, 0)))
	{
		CommonKnowledge = LoadGameInstance->CommonKnowledge;
		UE_LOG(LogTemp, Warning, TEXT("World Data Loaded from: %s"), *FullPath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load world data from slot: %s"), *FullPath);
	}
}

void ANPC_Manager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	SaveWorldData();
}

