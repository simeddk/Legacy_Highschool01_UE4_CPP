#include "CUserWidget_SelectAction.h"
#include "Global.h"
#include "CUserWidget_ActionIcon.h"
#include "Components/GridPanel.h"
#include "Components/Border.h"

void UCUserWidget_SelectAction::NativeConstruct()
{
	 TArray<UWidget*> children = Grid->GetAllChildren();
	 for (UWidget* child : children)
		 Items.Add(child->GetName(), Cast<UCUserWidget_ActionIcon>(child));

	Super::NativeConstruct();
}

void UCUserWidget_SelectAction::Pressed(FString InName)
{
	
}

void UCUserWidget_SelectAction::Hover(FString InName)
{
	UBorder* border = Cast<UBorder>(Items[InName]->GetWidgetFromName("IconBorder"));
	if (!!border)
		border->SetBrushColor(FLinearColor::Red);
	
}

void UCUserWidget_SelectAction::Unhover(FString InName)
{
	UBorder* border = Cast<UBorder>(Items[InName]->GetWidgetFromName("IconBorder"));
	if (!!border)
		border->SetBrushColor(FLinearColor::White);
}


