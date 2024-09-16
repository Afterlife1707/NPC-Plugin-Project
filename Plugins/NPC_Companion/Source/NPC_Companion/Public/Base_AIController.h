// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Base_AIController.generated.h"

/**
 * 
 */
UCLASS()
class NPC_COMPANION_API ABase_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	explicit ABase_AIController(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	class UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();
	FAISenseID SightID; 


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess))
	bool bIsCompanion;
	UFUNCTION(BlueprintCallable, Category="AI") 
	void ToggleFollowPlayer(bool Follow);//for story companion
	UFUNCTION(BlueprintCallable, Category="AI") 
	void TogglePatrol(bool Patrol);//for world npcs
};
