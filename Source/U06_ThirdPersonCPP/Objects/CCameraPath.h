#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCameraPath.generated.h"

UCLASS()
class U06_THIRDPERSONCPP_API ACCameraPath : public AActor
{
	GENERATED_BODY()
	
public:	
	ACCameraPath();

	FORCEINLINE class UCurveFloat* GetCurve() { return Curve; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USplineComponent* Spline;

	UPROPERTY(VisibleDefaultsOnly)
		class UCurveFloat* Curve;

};
