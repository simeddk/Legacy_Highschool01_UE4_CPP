#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterSpeedType : uint8
{
	Sneak, Walk, Run, Max
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U06_THIRDPERSONCPP_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStatusComponent();

	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetCurrentHealth() { return CurrentHealth; }

	FORCEINLINE float GetSneakSpeed()	{ return Speed[(int32)ECharacterSpeedType::Sneak]; }
	FORCEINLINE float GetWalkSpeed()	{ return Speed[(int32)ECharacterSpeedType::Walk]; }
	FORCEINLINE float GetRunSpeed()		{ return Speed[(int32)ECharacterSpeedType::Run]; }
	
	FORCEINLINE bool GetCanMove() { return bCanMove; }

	void SetMove();
	void SetStop();

	void SetSpeed(ECharacterSpeedType InType);

	void IncreaseHealth(float InAmount);
	void DecreaseHealth(float InAmount);

private:
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(int32)ECharacterSpeedType::Max] = { 200, 400, 600 };

protected:
	virtual void BeginPlay() override;

private:
	float CurrentHealth;
	bool bCanMove = true;
};
