// Crime Life Project, All Rights NOT Reserved.

#include "Menu/CLPMenuGameModeBase.h"

#include "Menu/CLPMenuPlayerController.h"
#include "Menu/UI/CLPMenuHUD.h"

ACLPMenuGameModeBase::ACLPMenuGameModeBase()
{
	PlayerControllerClass = ACLPMenuPlayerController::StaticClass();
	HUDClass			  = ACLPMenuHUD::StaticClass();
}
