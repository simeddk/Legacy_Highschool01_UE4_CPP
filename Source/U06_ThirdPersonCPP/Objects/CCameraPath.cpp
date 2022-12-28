#include "CCameraPath.h"
#include "Global.h"
#include "Components/SplineComponent.h"

ACCameraPath::ACCameraPath()
{
	CHelpers::CreateComponent<USplineComponent>(this, &Spline, "Spline");
	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/Objects/Curve_Unnormal.Curve_Unnormal'");
}

void ACCameraPath::BeginPlay()
{
	Super::BeginPlay();
	
}

