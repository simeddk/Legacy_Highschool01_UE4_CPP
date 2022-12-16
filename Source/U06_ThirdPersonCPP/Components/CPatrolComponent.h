#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPatrolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U06_THIRDPERSONCPP_API UCPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class ACPatrolPath* Path;

	UPROPERTY(EditAnywhere)
		int32 Index;

	UPROPERTY(EditAnywhere)
		bool bReverse;

	UPROPERTY(EditAnywhere)
		float AcceptanceRadius = 20.0f;

public:	
	UCPatrolComponent();

	bool GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadius);
	void UpdateNextIndex();

	FORCEINLINE bool IsValid() { return  Path != nullptr; }

protected:
	virtual void BeginPlay() override;

		
};
