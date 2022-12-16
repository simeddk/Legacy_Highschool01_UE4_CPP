#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Health.generated.h"

UCLASS()
class U06_THIRDPERSONCPP_API UCUserWidget_Health : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void Update(float InCurrentHealth, float InMaxHealth);
	
};
