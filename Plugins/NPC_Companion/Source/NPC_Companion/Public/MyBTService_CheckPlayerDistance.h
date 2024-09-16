// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "MyBTService_CheckPlayerDistance.generated.h"

/**
 * 
 */
UCLASS()
class NPC_COMPANION_API UMyBTService_CheckPlayerDistance : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	UMyBTService_CheckPlayerDistance();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
