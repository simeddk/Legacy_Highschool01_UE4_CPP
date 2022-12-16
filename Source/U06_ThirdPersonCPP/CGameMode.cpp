#include "CGameMode.h"
#include "Global.h"
#include "GameFramework/HUD.h"

ACGameMode::ACGameMode()
{
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "/Game/Player/BP_CPlayer");
	CHelpers::GetClass<AHUD>(&HUDClass, "/Game/Widgets/BP_CHUD");
}

