// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "NPC_Manager.generated.h"

UCLASS()
class NPC_COMPANION_API ANPC_Manager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPC_Manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	FString GameWorld;//a brief description of the game world.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	FString CommonKnowledge;//common knowledge that the npcs should know about the game world

	UPROPERTY(BlueprintReadWrite, Category="AI")
	TArray<AActor*> RegisteredNPCs;
	UPROPERTY(BlueprintReadWrite, Category="AI")
	FString NPC_Names;
	
	UFUNCTION(BlueprintCallable, Category="AI")
	void DynamicEvent(const FString& EventDescription);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess= "true"))
	FName OpenAI_APIKey;
private:
	void SaveWorldData();
	void LoadWorldData();
	FString WorldSaveFolder = FPaths::ProjectSavedDir() / TEXT("SaveGames/World");
	FString FullPath = WorldSaveFolder / TEXT("WorldSaveData");
};
