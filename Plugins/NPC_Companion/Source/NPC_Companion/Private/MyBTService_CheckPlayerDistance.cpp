// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_CheckPlayerDistance.h"

#include "NPC_Base.h"
#include "Base_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UMyBTService_CheckPlayerDistance::UMyBTService_CheckPlayerDistance()
{
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
	Interval = 0.5f;
}

void UMyBTService_CheckPlayerDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (auto* const Controller = Cast<ABase_AIController>(OwnerComp.GetAIOwner()))
	{
		if (Player)
		{
			FVector const PlayerLocation = Player->GetActorLocation();
			FVector const NPCLocation = Controller->GetPawn()->GetActorLocation();
			float const MinDistance = Cast<ANPC_Base>(Controller->GetCharacter())->MinDistance;
			float const SquaredDistance = FVector::DistSquared(PlayerLocation, NPCLocation);
			float const SquaredMinDistance = MinDistance * MinDistance;

			auto* BlackboardComp = OwnerComp.GetBlackboardComponent();
			if (!BlackboardComp) return;

			if (SquaredDistance >= SquaredMinDistance)
			{
				BlackboardComp->SetValueAsBool("bPlayRandomAnim", false);
			}
			else
			{
				BlackboardComp->SetValueAsBool("bPlayRandomAnim", true);
			}
		}
	}
}