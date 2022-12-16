#include "CBTTaskNode_Action.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
    NodeName = "Action";

    bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
    CheckNullResult(controller, EBTNodeResult::Failed);

    ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());
    CheckNullResult(aiPawn, EBTNodeResult::Failed);

    UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(aiPawn);
    action->DoAction();

    RunningTime = 0.f;

    return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
    CheckNull(controller);

    ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());
    CheckNull(aiPawn);

    UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);

    RunningTime += DeltaSeconds;

    if (state->IsIdleMode() && RunningTime > Delay)
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
