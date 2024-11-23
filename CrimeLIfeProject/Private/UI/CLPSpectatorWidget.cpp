// Crime Life Project, All Rights NOT Reserved.

#include "UI/CLPSpectatorWidget.h"

#include "Components/CLPRespawnComponent.h"

bool UCLPSpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
	if (!GetOwningPlayer())
	{
		return false;
	}

	const UCLPRespawnComponent* RespawnComponent = GetOwningPlayer()->FindComponentByClass<UCLPRespawnComponent>();
	if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress())
	{
		return false;
	}
	CountDownTime = RespawnComponent->GetRespawnCountDown();

	return true;
}
