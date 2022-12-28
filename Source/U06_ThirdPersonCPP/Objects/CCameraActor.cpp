#include "CCameraActor.h"
#include "Global.h"
#include "CCameraPath.h"
#include "Camera/CameraComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"

ACCameraActor::ACCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera");
}


void ACCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACCameraPath::StaticClass(), actors);

	for (AActor* actor : actors)
	{
		ACCameraPath* path = Cast<ACCameraPath>(actor);
		if (!!path)
			Path = path;
	}
}

void ACCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (Timeline.IsPlaying())
		Timeline.TickTimeline(DeltaTime);
}


void ACCameraActor::StartTimeline()
{
	CheckTrue(Timeline.IsPlaying());

	FOnTimelineFloat progress;
	progress.BindUFunction(this, "OnProgress");

	FOnTimelineEvent finish;
	finish.BindUFunction(this, "OnFinish");

	Timeline = FTimeline();
	Timeline.AddInterpFloat(Path->GetCurve(), progress);
	Timeline.SetTimelineFinishedFunc(finish);
	Timeline.SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
	Timeline.SetPlayRate(playRate);
	Timeline.PlayFromStart();

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	controller->SetViewTarget(this);
}

void ACCameraActor::OnProgress(float Output)
{
	USplineComponent* spline = CHelpers::GetComponent<USplineComponent>(Path);
	float length = spline->GetSplineLength();

	FVector location = spline->GetLocationAtDistanceAlongSpline(length * Output, ESplineCoordinateSpace::World);
	FRotator rotation = spline->GetRotationAtDistanceAlongSpline(length * Output, ESplineCoordinateSpace::World);

	SetActorLocation(location);
	SetActorRotation(rotation);
}

void ACCameraActor::OnFinish()
{
	Timeline.Stop();

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	controller->SetViewTarget(player);
}

