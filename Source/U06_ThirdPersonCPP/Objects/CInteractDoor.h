#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CInteractDoor.generated.h"

UCLASS()
class U06_THIRDPERSONCPP_API ACInteractDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACInteractDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void Interact();

private:
	void Opening(float DeltaTime);
	void Closing(float DeltaTime);

private:
	UPROPERTY(EditAnywhere)
		float Speed = 80.f;

	UPROPERTY(EditAnywhere)
		float MaxDegree = 90.f;

	UPROPERTY(EditAnywhere)
		bool bHiddenCollision = false;

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
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	bool bClosed = true;

	bool bOpening = false;
	bool bClosing = false;

	float Direction; //-1, +1
	float DottedDegree; //-90, +90

	float CurrentDegree; //점점 증감하는 값

	class ACPlayer* Player;
};
