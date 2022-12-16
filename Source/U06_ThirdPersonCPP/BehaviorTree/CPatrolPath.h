#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

UCLASS()
class U06_THIRDPERSONCPP_API ACPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPatrolPath();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class USplineComponent* Spline;

	UPROPERTY(VisibleDefaultsOnly)
		class UTextRenderComponent* Text;

public:
	FORCEINLINE class USplineComponent* GetSpline() { return Spline; }

private:
	UPROPERTY(EditAnywhere)
		bool bLoop;

};
