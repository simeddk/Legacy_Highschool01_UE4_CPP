#include "CUserWidget_ActionIcon.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "CUserWidget_SelectAction.h"

void UCUserWidget_ActionIcon::Pressed()
{
	GetSelecActionWidget()->Pressed(GetName());
}

void UCUserWidget_ActionIcon::Hover()
{
	GetSelecActionWidget()->Hover(GetName());
}

void UCUserWidget_ActionIcon::Unhover()
{
	GetSelecActionWidget()->Unhover(GetName());
}

UCUserWidget_SelectAction* UCUserWidget_ActionIcon::GetSelecActionWidget()
{
	ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	CheckNullResult(player, nullptr);

	return player->GetSelectActionWidget();
}