#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "CCameraActor.generated.h"

UCLASS()
class U06_THIRDPERSONCPP_API ACCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACCameraActor();

	UFUNCTION(BlueprintCallable)
		void StartTimeline();

	UFUNCTION()
		void OnProgress(float Output);

	UFUNCTION(BlueprintCallable)
		void OnFinish();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
		float playRate = 0.1f;

private:
	FTimeline Timeline;
	class ACCameraPath* Path;

};
