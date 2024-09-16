#include "MyBTTask_FollowPlayer.h"

#include "NPC_Base.h"
#include "Base_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UMyBTTask_FollowPlayer::UMyBTTask_FollowPlayer(FObjectInitializer const& ObjectInitializer) : UBTTask_BlackboardBase{ ObjectInitializer }
{
    NodeName = TEXT("Follow Player");
}

EBTNodeResult::Type UMyBTTask_FollowPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (auto* const Controller = Cast<ABase_AIController>(OwnerComp.GetAIOwner()))
    {
        if (Player)
        {
            // Get the player's location and velocity
            FVector const PlayerLocation = Player->GetActorLocation();
            FVector const PlayerVelocity = Player->GetVelocity();
            float const PlayerSpeed = PlayerVelocity.Size();
            float const MinDistance = Cast<ANPC_Base>(Controller->GetCharacter())->MinDistance;
            FVector const NPCLocation = Controller->GetPawn()->GetActorLocation();
            float const SquaredDistance = FVector::DistSquared(PlayerLocation, NPCLocation);
            float const SquaredMinDistance = MinDistance * MinDistance;

            auto* BlackboardComp = OwnerComp.GetBlackboardComponent();
            if (!BlackboardComp)
                return EBTNodeResult::Failed;

            BlackboardComp->SetValueAsBool("bPlayRandomAnim", false);

            // Check if the NPC needs to move closer to the player
            if (SquaredDistance >= SquaredMinDistance)
            {
                // Set walk speed based on whether the player is moving or not
                Controller->GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = (PlayerSpeed > 0.0f) ? PlayerSpeed : 400.f;
                constexpr float AdditionalProximity = 100.0f;
                // Calculate a location slightly closer to the player than MinDistance
                FVector const Direction = (NPCLocation - PlayerLocation).GetSafeNormal();
                FVector const TargetLocation = PlayerLocation + Direction * (MinDistance - AdditionalProximity);

                // Move the NPC to the adjusted target location near the player
                UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, TargetLocation);
            }
            else
            {
                // If the player is stationary and the NPC is within range, do not move the NPC and play a random anim
                Controller->StopMovement();
                BlackboardComp->SetValueAsBool("bPlayRandomAnim", true);
            }

            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            return EBTNodeResult::Succeeded;
        }
    }
    return EBTNodeResult::Failed;
}
