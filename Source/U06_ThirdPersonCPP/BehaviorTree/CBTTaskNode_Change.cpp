#include "CBTTaskNode_Change.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Change::UCBTTaskNode_Change()
{
	NodeName = "Change";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Change::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(aiPawn, EBTNodeResult::Failed);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(aiPawn);

	if (Type == EActionType::Warp)
	{
		if (action->IsWarpMode() == false)
			action->SetWarpMode();
	}
	
	if (Type == EActionType::MagicBall)
	{
		if (action->IsMagicBallMode() == false)
			action->SetMagicBallMode();
	}

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Change::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(aiPawn);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);

	if (state->IsIdleMode())
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
