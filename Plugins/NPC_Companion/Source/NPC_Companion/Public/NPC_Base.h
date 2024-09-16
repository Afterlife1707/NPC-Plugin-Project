// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Animation/AnimMontage.h"
#include "AzureEnums.h"
#include "NPC_Knowledge.h"
#include "NPC_Base.generated.h"

UCLASS(Blueprintable)
class NPC_COMPANION_API ANPC_Base : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		UBehaviorTree* GetBehaviourTree() const {return Tree;}

	UAnimMontage* GetMontage() const {return Montage;}

	UFUNCTION(BlueprintImplementableEvent, Category="AI")
	void UpdateKnowledge(const FString& NewKnowledge);

	UFUNCTION(BlueprintCallable, Category="Save Game")
	void SaveGame();

	bool DoesRecognizePlayer() const;
	FString GetNPC_Name() const {return NPC_Name.ToString();}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess= "true"))
	float MinDistance = 150.0f; //distance to be near the player (for companion)
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess= "true"))
	FName NPC_Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess= "true"))
	EAzureVoiceGender Gender;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess= "true"))
	EAzureVoiceName Voice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess= "true"))
	EAzureVoiceLanguage VoiceLanguage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess= "true"))
	FString NPC_Description; //include background with features such as age, gender, short description of the npc
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess= "true"))
	FString NPC_Purpose; //purpose of the npc in the game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess= "true"))
	FString NPC_TalkingStyle; //a talking style such as sassy, arrogant, polite, helpful, etc
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	TArray<FString> Traits;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess= "true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess= "true"))
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Save Game", meta=(AllowPrivateAccess= "true"))
	UNPC_Knowledge* SaveGameInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Save Game", meta=(AllowPrivateAccess= "true"))
	FPlayerInteractionData InteractionData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Save Game", meta=(AllowPrivateAccess= "true"))
	FString PlayerName;; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Save Game", meta=(AllowPrivateAccess= "true"))
	FString MessagesHistory;

	UPROPERTY(EditAnywhere, Category="Save Game", meta=(AllowPrivateAccess= "true"))
	FString NPCSaveSlot;

	FString NPCSaveFolder;
	FString FullPath;

public:	
	FVector GetPatrolPoint(int const index) const;
	int Num() const;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI, meta=(AllowPrivateAccess="true", MakeEditWidget="true"))
	TArray<FVector> PatrolPoints;
	void ConvertPatrolPointsToWorldSpace();

};
