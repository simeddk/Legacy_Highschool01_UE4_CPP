#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	Wait, Approach, Action, Patrol, Hitted, Avoid
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBehaviorTypeChanged, EBehaviorType, InPrevType, EBehaviorType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U06_THIRDPERSONCPP_API UCBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCBehaviorComponent();

	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

private:
	UPROPERTY(EditAnywhere)
		FName BehaviorKey = "Behavior";

	UPROPERTY(EditAnywhere)
		FName PlayerKey = "Player";

	UPROPERTY(EditAnywhere)
		FName WarpLocationKey = "WarpLocation";


public:
	UFUNCTION(BlueprintCallable)
		bool IsWaitMode();

	UFUNCTION(BlueprintCallable)
		bool IsApproachMode();

	UFUNCTION(BlueprintCallable)
		bool IsActionMode();

	UFUNCTION(BlueprintCallable)
		bool IsPatrolMode();

	UFUNCTION(BlueprintCallable)
		bool IsHittedMode();

	UFUNCTION(BlueprintCallable)
		bool IsAvoidMode();


public:
	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetAvoidMode();

	class ACPlayer* GetTargetPlayer();
	FVector GetWarpLocation();


protected:
	virtual void BeginPlay() override;


private:
	void ChangeType(EBehaviorType InType);
	EBehaviorType GetType();

private:
	class UBlackboardComponent* Blackboard;

public:
	UPROPERTY(BlueprintAssignable)
		FBehaviorTypeChanged OnBehaviorTypeChanged;
		
};
