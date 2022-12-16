#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOtherCharacter);

UCLASS()
class U06_THIRDPERSONCPP_API ACAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAttachment();

	void OnCollision();
	void OffCollision();

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		void AttachTo(FName InSocketName);

	UFUNCTION(BlueprintCallable)
		void AttachToCollision(class USceneComponent* InComponent, FName InSocketName);

	UFUNCTION(BlueprintImplementableEvent)
		void OnEquip();

	UFUNCTION(BlueprintImplementableEvent)
		void OnUnequip();

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(BlueprintAssignable)
		FAttachmentBeginOverlap OnAttachmentBeginOverlap;
	
	UPROPERTY(BlueprintAssignable)
		FAttachmentEndOverlap OnAttachmentEndOverlap;


protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

private:
	TArray<class UShapeComponent*> ShapeComponents;

};
