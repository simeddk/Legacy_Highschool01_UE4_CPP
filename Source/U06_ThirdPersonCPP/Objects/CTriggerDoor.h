#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTriggerDoor.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FTriggerDoorOpen, class ACharacter*);
DECLARE_MULTICAST_DELEGATE(FTriggerDoorClose);

UCLASS()
class U06_THIRDPERSONCPP_API ACTriggerDoor : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* DoorFrame;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Door;

	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;

private:
	UPROPERTY(EditAnywhere)
		float Speed = 0.02f;

	UPROPERTY(EditAnywhere)
		float MaxDegree = 90.0f;

	UPROPERTY(EditAnywhere)
		bool bHiddenCollision = false;
	
public:	
	ACTriggerDoor();

public:
	FTriggerDoorOpen OnTriggerDoorOpen;
	FTriggerDoorClose OnTriggerDoorClose;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void Open(class ACharacter* InCharacter);

	UFUNCTION()
		void Close();

private:
	bool bOpen = false;
	float MaxDegreeDirection;

};
