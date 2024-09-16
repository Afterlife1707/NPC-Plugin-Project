// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_FindPathPoint.h"

#include "NPC_Base.h"
#include "Base_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTTask_FindPathPoint::UMyBTTask_FindPathPoint(FObjectInitializer const& ObjectInitializer) :UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UMyBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const controller = Cast<ABase_AIController>(OwnerComp.GetAIOwner()))
	{
		// Attempt to get the blackboard component from the behaviour tree
		if (auto* const bc = OwnerComp.GetBlackboardComponent())
		{
			// Get the current patrol path index from the blackboard
			auto const index = bc->GetValueAsInt(GetSelectedBlackboardKey());

			// Get the NPC
			if (auto* npc = Cast<ANPC_Base>(controller->GetPawn()))
			{
				// Ensure there are patrol points
				if (npc->Num() == 0)
					return EBTNodeResult::Failed;

				// Convert the local vector to global point using the actor's transform
				auto const GlobalPoint = npc->GetPatrolPoint(index);
				bc->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

				// Finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}