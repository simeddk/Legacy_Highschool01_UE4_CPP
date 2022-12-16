#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Melee.generated.h"

UCLASS()
class U06_THIRDPERSONCPP_API ACDoAction_Melee : public ACDoAction
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo() { bCanCombo = true; }
	FORCEINLINE void DisableCombo() { bCanCombo = false; }
	
public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

public:
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;

private:
	UFUNCTION()
		void RestoreGlobalTimeDilation();

private:
	bool bCanCombo;
	bool bSucceed;

	int32 ComboCount;

	TArray<class ACharacter*> HittedCharacters;
};
