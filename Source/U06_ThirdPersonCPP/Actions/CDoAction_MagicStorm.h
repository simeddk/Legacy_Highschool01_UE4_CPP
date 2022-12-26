#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_MagicStorm.generated.h"

UCLASS()
class U06_THIRDPERSONCPP_API ACDoAction_MagicStorm : public ACDoAction
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;
	
	virtual void Abort() override;

	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void Hitted();

	UFUNCTION()
		void Finish();

private:
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;

private:
	UPROPERTY(EditDefaultsOnly)
		float DPS = 0.1f;

	UPROPERTY(EditDefaultsOnly)
		float ActivationTime = 5.f;

	UPROPERTY(EditDefaultsOnly)
		float Speed = 300.f;

	UPROPERTY(EditDefaultsOnly)
		float Distance = 150.f;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle;

private:
	class UBoxComponent* Box;
	bool bActivating;

	TArray<class ACharacter*> HittedCharacters;
	float Angle;
};
