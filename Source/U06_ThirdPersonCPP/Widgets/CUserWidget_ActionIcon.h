#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_ActionIcon.generated.h"

UCLASS()
class U06_THIRDPERSONCPP_API UCUserWidget_ActionIcon : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
		void Pressed();

	UFUNCTION(BlueprintCallable)
		void Hover();

	UFUNCTION(BlueprintCallable)
		void Unhover();

private:
	class UCUserWidget_SelectAction* GetSelecActionWidget();
};
