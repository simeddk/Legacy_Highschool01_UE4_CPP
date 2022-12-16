#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class U06_THIRDPERSONCPP_API ACAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACAIController();
	virtual void Tick(float DeltaTime) override;

	float GetSightRadius();
	FORCEINLINE float GetBehaviorRange() { return BehaviorRange; }

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCBehaviorComponent* Behavior;

	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;

private:
	class ACEnemy_AI* OwnerEnemy;
	class UAISenseConfig_Sight* Sight;

private:
	UPROPERTY(EditAnywhere)
		float BehaviorRange = 150.f;

	UPROPERTY(EditAnywhere)
		bool bDrawCircle = true;

	UPROPERTY(EditAnywhere)
		float HeightOffset = 50.0f;
};
