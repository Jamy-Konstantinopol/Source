// Crime Life Project, All Rights NOT Reserved.

#include "CLPGameInstance.h"

const TArray<FLevelItemDataWidget>& UCLPGameInstance::GetLevelItemDataWidgets() const
{
	return LevelItemDataWidgets;
}

FName UCLPGameInstance::GetMainMenuLevelName() const
{
	return MainMenuLevelName;
}
