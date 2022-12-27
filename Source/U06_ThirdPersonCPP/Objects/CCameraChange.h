#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCameraChange.generated.h"

UCLASS()
class U06_THIRDPERSONCPP_API ACCameraChange : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly)
		TArray<class ACameraActor*> Cameras;
	
public:	
	ACCameraChange();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void Change();

private:
	uint32 Index;

};
