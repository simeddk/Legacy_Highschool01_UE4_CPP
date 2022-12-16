#include "CAim.h"
#include "Global.h"
#include "Widgets/CHUD.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

UCAim::UCAim()
{
	CHelpers::GetAsset<UCurveFloat>(&Curve, "/Game/Actions/Curve_Aim");
}

void UCAim::BeginPlay(ACharacter* InOwnerCharacter)
{
	OwnerCharacter = InOwnerCharacter;

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);

	OnTimelineFloat.BindUFunction(this, "Zooming");
	Timeline.AddInterpFloat(Curve, OnTimelineFloat);
	Timeline.SetPlayRate(200.f);

	Hud = InOwnerCharacter->GetWorld()->GetFirstPlayerController()->GetHUD<ACHUD>();
}

void UCAim::Tick(float DeltaTime)
{
	Timeline.TickTimeline(DeltaTime);
}

void UCAim::On()
{
	CheckFalse(IsAvaliable());
	CheckTrue(bZooming);

	bZooming = true;

	Hud->EnableVisible();

	SpringArm->TargetArmLength = 100.f;
	SpringArm->SocketOffset = FVector(0, 30, 10);
	SpringArm->bEnableCameraLag = false;

	Timeline.PlayFromStart();
}

void UCAim::Off()
{
	CheckFalse(IsAvaliable());
	CheckFalse(bZooming);

	bZooming = false;

	Hud->DisableVisible();

	SpringArm->TargetArmLength = 200.f;
	SpringArm->SocketOffset = FVector(0, 0, 0);
	SpringArm->bEnableCameraLag = true;

	Timeline.ReverseFromEnd();
	
}

void UCAim::Zooming(float Output)
{
	Camera->FieldOfView = Output;
}
