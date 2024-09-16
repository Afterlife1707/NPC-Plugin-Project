// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_AIController.h"

#include "NPC_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ABase_AIController::ABase_AIController(FObjectInitializer const& ObjectInitializer)
{
	//SetupPerceptionSystem();
}

void ABase_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if(ANPC_Base* const npc=Cast<ANPC_Base>(InPawn))
	{
		if(UBehaviorTree* const tree = npc->GetBehaviourTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset,b);
			Blackboard = b;
			RunBehaviorTree(tree);
		}
	}
}


///UNUSED SINCE WE ALWAYS KNOW PLAYER LOCATION/// CAN BE USED FOR OTHER CASES LATER
void ABase_AIController::SetupPerceptionSystem()
{
    //create and setup hear and sight senses
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    if(SightConfig)
    {
        SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
        SightConfig->SightRadius = 1000.f;
        SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
        SightConfig->PeripheralVisionAngleDegrees = 90.f;
        SightConfig->SetMaxAge(5.f);
        SightConfig->AutoSuccessRangeFromLastSeenLocation = 1020.f;
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

        GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
        GetPerceptionComponent()->ConfigureSense(*SightConfig);
        SightID = SightConfig->GetSenseID();
    }
	
   // GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABase_AIController::OnTargetDetected);
}

void ABase_AIController::ToggleFollowPlayer(bool Follow)
{
	GetBlackboardComponent()->SetValueAsBool("bFollowPlayer", Follow);
}

void ABase_AIController::TogglePatrol(bool Patrol)
{
	GetBlackboardComponent()->SetValueAsBool("bShouldPatrol", Patrol);
}
