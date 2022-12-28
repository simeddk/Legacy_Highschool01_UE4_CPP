#include "CCameraChange.h"
#include "Global.h"
#include "Camera/CameraActor.h"

ACCameraChange::ACCameraChange()
{

}

void ACCameraChange::BeginPlay()
{
	Super::BeginPlay();
	
	/*TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), actors);
	
	for (AActor* actor : actors)
	{
		ACameraActor* camera = Cast<ACameraActor>(actor);
		if (!!camera)
			Cameras.Add(camera);
	}*/

	UKismetSystemLibrary::K2_SetTimer(this, "Change", 2.f, true);
}

void ACCameraChange::Change()
{
	/*APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	CheckNull(cameraManager);

	Index++;
	Index %= Cameras.Num();

	cameraManager->SetViewTarget(Cameras[Index]);*/
	
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (Cameras.Num() < 1) return;

	controller->SetViewTargetWithBlend(Cameras[Index], 2.f, EViewTargetBlendFunction::VTBlend_EaseIn, 2.f);

	Index++;
	Index %= Cameras.Num();
}

