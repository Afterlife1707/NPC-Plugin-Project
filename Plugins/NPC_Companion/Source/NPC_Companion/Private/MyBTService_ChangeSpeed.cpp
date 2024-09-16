// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_ChangeSpeed.h"

#include "NPC_Base.h"
#include "Base_AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UMyBTService_ChangeSpeed::UMyBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UMyBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	if (auto* const Controller = OwnerComp.GetAIOwner())
	{
		if (ANPC_Base* const NPC = Cast<ANPC_Base>(Controller->GetPawn()))
		{
			NPC->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
}