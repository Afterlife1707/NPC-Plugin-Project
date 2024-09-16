#include "MyBTTask_PlayRandomAnim.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"

UMyBTTask_PlayRandomAnim::UMyBTTask_PlayRandomAnim(): CachedOwnerComp(nullptr)
{
    NodeName = "Play Random Animation";
}

EBTNodeResult::Type UMyBTTask_PlayRandomAnim::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    CachedOwnerComp = &OwnerComp;
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;
	
    ACharacter* Character = Cast<ACharacter>(AIController->GetPawn());
    if (!Character) return EBTNodeResult::Failed;
	
    if (Animations.Num() == 0) return EBTNodeResult::Failed;

    int32 RandomIndex = FMath::RandRange(0, Animations.Num() - 1);
    if (UAnimSequence* SelectedAnimation = Animations[RandomIndex])
    {
        if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
        {
            AnimInstance->OnMontageEnded.RemoveDynamic(this, &UMyBTTask_PlayRandomAnim::OnAnimationFinished);
            if (UAnimMontage* Montage = AnimInstance->PlaySlotAnimationAsDynamicMontage(Cast<UAnimSequenceBase>(SelectedAnimation), FName("DefaultSlot")))
            {
                AnimInstance->OnMontageEnded.AddDynamic(this, &UMyBTTask_PlayRandomAnim::OnAnimationFinished);
                return EBTNodeResult::InProgress;  // Task is still running until animation ends
            }
        }
    }

    return EBTNodeResult::Failed;
}

void UMyBTTask_PlayRandomAnim::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
    EBTNodeResult::Type TaskResult)
{
    if (AAIController* AIController = OwnerComp.GetAIOwner())
    {
        if (ACharacter* Character = Cast<ACharacter>(AIController->GetPawn()))
        {
            if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
            {
                // Remove the delegate correctly
                AnimInstance->OnMontageEnded.RemoveDynamic(this, &UMyBTTask_PlayRandomAnim::OnAnimationFinished);
            }
        }
    }
    CachedOwnerComp = nullptr;
}

void UMyBTTask_PlayRandomAnim::OnAnimationFinished(UAnimMontage* Montage, bool bInterrupted)
{
    if (CachedOwnerComp)
    {
        FinishLatentTask(*CachedOwnerComp, bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);
        // Unbind the delegate after the task finishes
        if (AAIController* AIController = CachedOwnerComp->GetAIOwner())
        {
            if (ACharacter* Character = Cast<ACharacter>(AIController->GetPawn()))
            {
                if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
                {
                    AnimInstance->OnMontageEnded.RemoveDynamic(this, &UMyBTTask_PlayRandomAnim::OnAnimationFinished);
                }
            }
        }
    }
}

EBTNodeResult::Type UMyBTTask_PlayRandomAnim::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (AAIController* AIController = OwnerComp.GetAIOwner())
    {
        if (ACharacter* Character = Cast<ACharacter>(AIController->GetPawn()))
        {
            if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
            {
                // Stop the animation and remove the delegate
                AnimInstance->Montage_Stop(0.2f);
                AnimInstance->OnMontageEnded.RemoveDynamic(this, &UMyBTTask_PlayRandomAnim::OnAnimationFinished);
            }
        }
    }
    FinishLatentAbort(OwnerComp);
    CachedOwnerComp = nullptr;
    return EBTNodeResult::Succeeded;
}