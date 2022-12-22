// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_SelectAction.generated.h"

UCLASS()
class U06_THIRDPERSONCPP_API UCUserWidget_SelectAction : public UUserWidget
{
	GENERATED_BODY()

public:
	void Pressed(FString InName);
	void Hover(FString InName);
	void Unhover(FString InName);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UGridPanel* Grid;

	UPROPERTY(BlueprintReadOnly)
		TMap<FString, class UCUserWidget_ActionIcon*> Items;
};
