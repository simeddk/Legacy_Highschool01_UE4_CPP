#include "CBTService_Wizard.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"

UCBTService_Wizard::UCBTService_Wizard()
{
	NodeName = "Wizard";
}

void UCBTService_Wizard::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(aiPawn);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);

	CheckTrue(state->IsDeadMode());

	if (state->IsHittedMode())
	{
		behavior->SetHittedMode();
		return;
	}

	ACPlayer* target = behavior->GetTargetPlayer();
	if (target == nullptr)
	{
		behavior->SetWaitMode();
		controller->ClearFocus(EAIFocusPriority::Gameplay);

		return;
	}
	else
	{
		UCStateComponent* playerState = CHelpers::GetComponent<UCStateComponent>(target);
		if (playerState->IsDeadMode())
		{
			behavior->SetWaitMode();
			return;
		}
	}

	controller->SetFocus(target);

	float distance = aiPawn->GetDistanceTo(target);
	if (distance < controller->GetBehaviorRange())
	{
		behavior->SetAvoidMode();
		return;
	}

	if (distance < controller->GetSightRadius())
	{
		behavior->SetActionMode();
		return;
	}
}
