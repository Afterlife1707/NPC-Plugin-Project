// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyBTTask_PlayRandomAnim.generated.h"

/**
 * 
 */
UCLASS()
class NPC_COMPANION_API UMyBTTask_PlayRandomAnim : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	UMyBTTask_PlayRandomAnim();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	TArray<class UAnimSequence*> Animations;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	FDelegateHandle AnimationEndDelegateHandle;
	UFUNCTION()
	void OnAnimationFinished(UAnimMontage* Montage, bool bInterrupted);
	UBehaviorTreeComponent* CachedOwnerComp;
};
