#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CHUD.generated.h"

UCLASS()
class U06_THIRDPERSONCPP_API ACHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ACHUD();

	virtual void DrawHUD() override;

public:
	FORCEINLINE void EnableVisible() { bVisible = true; }
	FORCEINLINE void DisableVisible() { bVisible = false; }

private:
	UPROPERTY(EditDefaultsOnly)
		class UTexture2D* Texture;

private:
	bool bVisible;

};
