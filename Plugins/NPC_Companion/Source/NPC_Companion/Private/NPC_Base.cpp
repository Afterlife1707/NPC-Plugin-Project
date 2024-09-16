// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Base.h"

#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "HAL/FileManager.h"

// Sets default values
ANPC_Base::ANPC_Base()
{
	ANPC_Base::StaticClass();
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SaveGameInstance = nullptr;
}

// Called when the game starts or when spawned
void ANPC_Base::BeginPlay()
{
	Super::BeginPlay();
		ConvertPatrolPointsToWorldSpace();
	MessagesHistory = "None";
	
	NPCSaveSlot = FString::Printf(TEXT("%s_SaveSlot"), *NPC_Name.ToString());
	NPCSaveFolder = FPaths::ProjectSavedDir() / TEXT("SaveGames/NPCs");
	FullPath = NPCSaveFolder / NPCSaveSlot;
	
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (APlayerState* PS = PC->GetPlayerState<APlayerState>())
		{
			PlayerName = PS->GetPlayerName();
		}
	}
	
	if (UGameplayStatics::DoesSaveGameExist(NPCSaveSlot, 0))
	{
		SaveGameInstance = Cast<UNPC_Knowledge>(UGameplayStatics::LoadGameFromSlot(NPCSaveSlot, 0));
		if (SaveGameInstance)
		{
			InteractionData = SaveGameInstance->LoadInteractionData(NPCSaveSlot);
			if (DoesRecognizePlayer())
			{
				FString InteractionDataString = InteractionData.ToString();
				UE_LOG(LogTemp, Log, TEXT("player : %s"),*PlayerName);
				UE_LOG(LogTemp, Log, TEXT("Loaded Interaction Data: %s"), *InteractionDataString);
				MessagesHistory = *InteractionDataString;
			}
			else
			{
				// Handle new player case
				UE_LOG(LogTemp, Log, TEXT("new player"));
				InteractionData = FPlayerInteractionData();
				InteractionData.PlayerName = PlayerName;
				SaveGameInstance->SaveInteractionData(InteractionData, NPCSaveSlot);
			}
		}
	}
	else
	{
		// Create a new save game object if none exists
		SaveGameInstance = Cast<UNPC_Knowledge>(UGameplayStatics::CreateSaveGameObject(UNPC_Knowledge::StaticClass()));
		if (SaveGameInstance)
		{
			InteractionData = FPlayerInteractionData();
			InteractionData.PlayerName = PlayerName;
			SaveGameInstance->SaveInteractionData(InteractionData,NPCSaveSlot);
		}
	}
}

// Called every frame
void ANPC_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANPC_Base::SaveGame()
{
	if (SaveGameInstance)
	{
		IFileManager::Get().MakeDirectory(*NPCSaveFolder, true);
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, FullPath, 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("NPC Game Saved to: %s"), *FullPath);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save game to slot: %s"), *FullPath);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SaveGameInstance is nullptr, cannot save game!"));
	}
}

bool ANPC_Base::DoesRecognizePlayer() const
{
	return InteractionData.PlayerName == PlayerName;
}

FVector ANPC_Base::GetPatrolPoint(int const index) const
{
	return PatrolPoints[index];
}

int ANPC_Base::Num() const
{
	return PatrolPoints.Num();
}

void ANPC_Base::ConvertPatrolPointsToWorldSpace()
{
	TArray<FVector> WorldSpacePoints;

	FTransform ActorTransform = GetActorTransform();
	// Convert each patrol point to world space
	for (const FVector& LocalPoint : PatrolPoints)
	{
		FVector WorldPoint = ActorTransform.TransformPosition(LocalPoint);
		WorldSpacePoints.Add(WorldPoint);
	}

	
	PatrolPoints = WorldSpacePoints;
}



