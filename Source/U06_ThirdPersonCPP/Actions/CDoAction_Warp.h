#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Warp.generated.h"

UCLASS()
class U06_THIRDPERSONCPP_API ACDoAction_Warp : public ACDoAction
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void Tick(float DeltaTime) override;

private:
	bool GetDecalLocation(FVector& OutLocation, FRotator& OutRotator);
	class ACPlayer* GetPlayerUsing();

private:
	class UDecalComponent* Decal;
	FVector Location;
	
};
