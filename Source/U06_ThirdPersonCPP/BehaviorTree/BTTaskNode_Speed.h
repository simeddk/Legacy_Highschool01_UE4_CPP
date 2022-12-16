#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CStatusComponent.h"
#include "BTTaskNode_Speed.generated.h"

UCLASS()
class U06_THIRDPERSONCPP_API UBTTaskNode_Speed : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		ECharacterSpeedType Type;

public:
	UBTTaskNode_Speed();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
