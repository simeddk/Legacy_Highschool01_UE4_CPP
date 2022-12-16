#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/TimelineComponent.h"
#include "CAim.generated.h"

UCLASS()
class U06_THIRDPERSONCPP_API UCAim : public UObject
{
	GENERATED_BODY()

public:
	UCAim();

	FORCEINLINE bool IsAvaliable() { return SpringArm != nullptr && Camera != nullptr; }
	FORCEINLINE bool IsZooming() { return bZooming; }

public:
	void BeginPlay(class ACharacter* InOwnerCharacter);
	void Tick(float DeltaTime);
	
	void On();
	void Off();

private:
	UFUNCTION()
		void Zooming(float Output);

private:
	UPROPERTY(EditAnywhere)
		class UCurveFloat* Curve;

private:
	class ACharacter* OwnerCharacter;
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

	bool bZooming;

	FTimeline Timeline;
	FOnTimelineFloat OnTimelineFloat;

	class ACHUD* Hud;
};
