#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CFeetComponent.generated.h"

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector LeftDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector RightDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector PelvisDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator LeftRotator;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator RightRotator;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U06_THIRDPERSONCPP_API UCFeetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCFeetComponent();
	
	FORCEINLINE FFeetData GetData() { return Data; }

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void Trace(FName InSocketName, float& OutDistance, FRotator& OutRotator);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		FName LeftSocket = "Foot_L";

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		FName RightSocket = "Foot_R";

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		float AdditionalDistance = 55.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForOneFrame;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		float FootOffset = 5.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "IK")
		float InterpSpeed = 20.f;

private:
	FFeetData Data;

	class ACharacter* OwnerCharacter;
	float CapsuleHalfHeight;
};
